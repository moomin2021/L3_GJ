#pragma once
// --DirectXInput-- //
#define DIREXTINPUT_VERSION 0x0800// DirectInput�̃o�[�W�����w��
#include <wrl.h>
#include <dinput.h>
#include <vector>

class Key {
	// namespace�̏ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region �����o�ϐ�
private:
	// ���͏��
	std::vector<uint8_t> keys_;		// ���݂̃L�[�{�[�h�̏��
	std::vector<uint8_t> oldKeys_;	// �O�t���[���̃L�[�{�[�h�̏��

	// �f�o�C�X
	ComPtr<IDirectInputDevice8> device_;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns> �C���X�^���X </returns>
	static Key* GetInstance();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �w�肳�ꂽ�L�[��������Ă�����A1��Ԃ��A�����łȂ�������0��Ԃ�
	/// </summary>
	/// <param name="key"> ���肵�����L�[ </param>
	/// <returns></returns>
	bool PushKey(uint16_t key) { return keys_[key]; }

	/// <summary>
	/// �w�肳�ꂽ�L�[�������ꂽ�u�Ԃ�������A1��Ԃ��A�����łȂ�������0��Ԃ�
	/// </summary>
	/// <param name="key"> ���肵�����L�[ </param>
	/// <returns></returns>
	bool TriggerKey(uint16_t key) { return keys_[key] && !oldKeys_[key]; }

private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Key();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Key();
#pragma endregion

#pragma region ����֐�
	// �֎~
	Key(const Key&) = delete;				// �R�s�[�R���X�g���N�^�֎~
	Key& operator = (const Key&) = delete;	// �R�s�[������Z�q�֎~
#pragma endregion
};