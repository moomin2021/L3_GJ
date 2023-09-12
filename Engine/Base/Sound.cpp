#include "Sound.h"

#include <cassert>

Sound* Sound::GetInstance()
{
	static Sound inst;

	return &inst;
}

void Sound::Initialize()
{
	HRESULT result;

	// XAudio�G���W���̃C���X�^���X���쐬
	result = XAudio2Create(&pXAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	// �}�X�^�����O�������쐬
	result = pXAudio2_->CreateMasteringVoice(&pMasterVoice_);
	assert(SUCCEEDED(result));
}

void Sound::Update()
{
	for (auto it = isPlaySounds_.begin(); it != isPlaySounds_.end();) {
		XAUDIO2_VOICE_STATE state;
		it->second->GetState(&state);

		if (state.BuffersQueued <= 0) {
			it->second->Stop(0);
			it = isPlaySounds_.erase(it);
		}

		else {
			++it;
		}
	}
}

uint16_t Sound::LoadWave(std::string fileName, float volume)
{
	if (soundHandles_.count(fileName) > 0) {
		return soundHandles_[fileName];
	}

	soundCounter_++;

	// �t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;

	// .wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(fileName, std::ios_base::binary);

	// �t�@�C���I�[�v�����s�����o����
	assert(file.is_open());

	// RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	// �t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}

	// �^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	// Format�`�F�b�N�̓ǂݍ���
	FormatChunk format = {};

	// �`�����N�w�b�_�[�̊m�F
	file.read((char*)&format, sizeof(ChunkHeader));
	if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
		assert(0);
	}

	// �`�����N�{�̂̓ǂݍ���
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	// Data�`�����N�̓ǂݍ���
	ChunkHeader data{};
	file.read((char*)&data, sizeof(data));

	// JUNK�`�����N�����o�����ꍇ
	if (strncmp(data.id, "JUNK", 4) == 0)
	{
		// �ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, std::ios_base::cur);

		// �ēǂݍ���
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0) {
		assert(0);
	}

	// Data�`�����N�̃f�[�^���i�g�`�f�[�^�j�̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	// �t�@�C�������
	file.close();

	// return����ׂ̉����f�[�^
	SoundData soundData = {};
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;
	soundData.volume = volume;

	// �����f�[�^�ۑ�
	soundDatas_.emplace(soundCounter_, soundData);
	soundHandles_.emplace(fileName, soundCounter_);

	return soundCounter_;
}

void Sound::Play(uint16_t handle, bool isLoop)
{
	HRESULT result;

	if (isPlaySounds_.count(handle)) {
		for (auto it = isPlaySounds_.begin(); it != isPlaySounds_.end();) {
			if (it->first == handle) {
				it->second->Stop(0);
				it = isPlaySounds_.erase(it);
				break;
			}
		}
	}

	// �\�[�X�������쐬
	IXAudio2SourceVoice* sourceVoices = nullptr;
	result = pXAudio2_->CreateSourceVoice(&sourceVoices, &soundDatas_[handle].wfex);
	assert(SUCCEEDED(result));

	// �\�[�X�����Ƀo�b�t�@�𑗐M
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundDatas_[handle].pBuffer;
	buf.AudioBytes = soundDatas_[handle].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (isLoop) buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	result = sourceVoices->SubmitSourceBuffer(&buf);

	// �Đ�
	result = sourceVoices->SetVolume(soundDatas_[handle].volume);
	result = sourceVoices->Start(0);
	assert(SUCCEEDED(result));

	isPlaySounds_.emplace(handle, sourceVoices);
}

void Sound::Stop(uint16_t handle)
{
	for (auto it = isPlaySounds_.begin(); it != isPlaySounds_.end();) {
		if (it->first == handle) {
			it->second->Stop(0);
			it = isPlaySounds_.erase(it);
			break;
		}
	}
}

void Sound::Release()
{
	for (auto& it : isPlaySounds_) {
		it.second->Stop(0);
	}

	isPlaySounds_.clear();
	soundDatas_.clear();
	soundHandles_.clear();
	soundCounter_ = 0;
}