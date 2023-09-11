#pragma once
#include <xaudio2.h>
#include <fstream>
#include <memory>
#include <wrl.h>
#include <map>
#include <string>

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

class Sound {
private:
	// �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
#pragma region �����o�ϐ�
private:
	// XAudio2�G���W���̃C���X�^���X
	ComPtr<IXAudio2> pXAudio2_ = nullptr;

	// �}�X�^�����O����
	IXAudio2MasteringVoice* pMasterVoice_ = nullptr;

	// �ǂݍ��񂾉����f�[�^
	std::map<uint16_t, SoundData> soundDatas_ = {};

	// �����n���h����ۑ�
	std::map<std::string, uint16_t> soundHandles_ = {};

	// �����ǂݍ��݃J�E���^�[
	uint16_t soundCounter_ = 0;
#pragma endregion

#pragma region �����o�֐�
public:
	// �C���X�^���X�擾
	static Sound* GetInstance();

	// ����������
	void Initialize();

	// �T�E���h�ǂݍ���
	uint16_t LoadWave(std::string fileName);

	// �Đ�
	void Play(uint16_t handle);
#pragma endregion
};