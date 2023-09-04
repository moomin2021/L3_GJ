#include "Sound.h"
#include <cassert>

Microsoft::WRL::ComPtr<IXAudio2> Sound::pXAudio2_ = nullptr;
std::map<uint16_t, IXAudio2SourceVoice*> Sound::sourceVoices_;
uint16_t Sound::sourceVoiceCount_ = 0;

Sound* Sound::GetInstance()
{
	// �C���X�^���X����
	static Sound inst;

	// �C���X�^���X��Ԃ�
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

uint16_t Sound::LoadWave(const char* filename) {
	// �t�@�C�����̓X�g���[���̃C���X�^���X
	std::ifstream file;

	// .wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(filename, std::ios_base::binary);

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

	HRESULT result;

	// �\�[�X�������쐬
	sourceVoices_[sourceVoiceCount_] = nullptr;
	result = pXAudio2_->CreateSourceVoice(&sourceVoices_[sourceVoiceCount_], &soundData.wfex);
	assert(SUCCEEDED(result));

	// �\�[�X�����Ƀo�b�t�@�𑗐M
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	result = sourceVoices_[sourceVoiceCount_]->SubmitSourceBuffer(&buf);

	sourceVoiceCount_++;

	return sourceVoiceCount_ - 1;
}

void Sound::Play(uint16_t sourceVoiceKey)
{
	HRESULT result;

	// �Đ�
	result = sourceVoices_[sourceVoiceKey]->Start(0);
	assert(SUCCEEDED(result));
}

void Sound::Stop(uint16_t sourceVoiceKey)
{
	HRESULT result;

	// ��~
	result = sourceVoices_[sourceVoiceKey]->Stop(0);
	assert(SUCCEEDED(result));
}

void Sound::SetVolume(uint16_t sourceVoiceKey, float volumeValue) {
	HRESULT result;

	// ���ʒ���
	result = sourceVoices_[sourceVoiceKey]->SetVolume(volumeValue);
	assert(SUCCEEDED(result));
}

Sound::Sound() {}