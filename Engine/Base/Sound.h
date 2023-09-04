#pragma once
#include <xaudio2.h>
#include <fstream>
#include <memory>
#include <wrl.h>
#include <map>

// �`�����N�w�b�_
struct ChunkHeader {
	char id[4];		// �`�����N����ID
	uint32_t size;	// �`�����N�T�C�Y
};

// RIFF�w�b�_�`�����N
struct RiffHeader {
	ChunkHeader chunk;	// RIFF
	char type[4];		// WAVE
};

// FMT�`�����N
struct FormatChunk {
	ChunkHeader chunk;	// fmt
	WAVEFORMATEX fmt;	// �g�`�t�H�[�}�b�g
};

// �����f�[�^
struct SoundData {
	WAVEFORMATEX wfex;	// �g�`�t�H�[�}�b�g
	BYTE* pBuffer;		// �o�b�t�@�̐擪�A�h���X
	uint32_t bufferSize;// �o�b�t�@�T�C�Y
};

class Sound
{
private:
	// �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region �����o�ϐ�
private:
	// XAudio2�G���W���̃C���X�^���X
	static ComPtr<IXAudio2> pXAudio2_;

	// �}�X�^�����O����
	IXAudio2MasteringVoice* pMasterVoice_ = nullptr;

	// �\�[�X�����A�z�z��ŕۑ�
	static std::map<uint16_t, IXAudio2SourceVoice*> sourceVoices_;

	// �\�[�X���������J�E���g
	static uint16_t sourceVoiceCount_;
#pragma endregion

#pragma region �����o�֐�
public:
	// �C���X�^���X�擾
	static Sound* GetInstance();

	// ����������
	void Initialize();

	// �T�E���h�ǂݍ���
	static uint16_t LoadWave(const char* filename);

	// �T�E���h�̍Đ�
	static void Play(uint16_t sourceVoiceKey);

	// �T�E���h�̒�~
	static void Stop(uint16_t sourceVoiceKey);

	// �T�E���h�̉��ʒ���
	static void SetVolume(uint16_t sourceVoiceKey, float volumeValue);

private:
	// �R���X�g���N�^
	Sound();
#pragma endregion
};