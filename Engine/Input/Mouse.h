#pragma once
// DirectXInput
#include <dinput.h>

// ComPtr�p
#include <wrl.h>

// Vector2
#include "Vector2.h"

enum MouseButton {
	M_LEFT,
	M_RIGHT,
	M_CENTER
};

class Mouse {
private:
	// �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region �����o�ϐ�
private:
	// �f�o�C�X
	ComPtr<IDirectInputDevice8> device_;

	// �}�E�X�f�[�^
	DIMOUSESTATE nowMouse_;// ���݂̃}�E�X�̓��͏��
	DIMOUSESTATE oldMouse_;// �O�t���[���̃}�E�X�̓��͏��

	// �}�E�X�J�[�\�����W
	POINT p_;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns> �C���X�^���X </returns>
	static Mouse* GetInstance();

	/// <summary>
	/// �X�V����
	/// </summary>
	void Update();

	/// <summary>
	/// �w�肳�ꂽ�}�E�X�̃{�^����������Ă�����A1��Ԃ��A�����łȂ������ꍇ0��Ԃ�
	/// </summary>
	/// <param name="button"> ���肵�����}�E�X�{�^�� </param>
	/// <returns></returns>
	bool PushMouseButton(MouseButton button) { return nowMouse_.rgbButtons[button] == (0x80); }

	/// <summary>
	/// �w�肳�ꂽ�}�E�X�̃{�^���������ꂽ�u�Ԃ�������A1��Ԃ��A�����łȂ������ꍇ0��Ԃ�
	/// </summary>
	/// <param name="button"> ���肵�����}�E�X�{�^�� </param>
	/// <returns></returns>
	bool TriggerMouseButton(MouseButton button) { return nowMouse_.rgbButtons[button] == (0x80) && !(oldMouse_.rgbButtons[button] == (0x80)); }

	/// <summary>
	/// �}�E�X�̈ʒu���W
	/// </summary>
	/// <returns> �}�E�X�̈ʒu���W </returns>
	Vector2 MousePos() { return Vector2(static_cast<float>(p_.x), static_cast<float>(p_.y)); }

	/// <summary>
	/// �}�E�X�̈ړ���
	/// </summary>
	/// <returns> �}�E�X�̈ړ��� </returns>
	Vector2 GetMouseVelosity() { return Vector2((float)nowMouse_.lX, (float)nowMouse_.lY); }

private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Mouse();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Mouse();
#pragma endregion

#pragma region ����֐�
	// �֎~
	Mouse(const Mouse&) = delete;				// �R�s�[�R���X�g���N�^�֎~
	Mouse& operator = (const Mouse&) = delete;	// �R�s�[������Z�q�֎~
#pragma endregion
};