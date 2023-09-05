#pragma once
// DirectInput�o�[�W�����w��
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <Xinput.h>
#include <wrl.h>
#include <stdint.h>

#include "Vector2.h"

// �{�^���}�X�N
enum BUTTON {
	PAD_UP = 0x0001,// �\���L�[��
	PAD_DOWN = 0x0002,// �\���L�[��
	PAD_LEFT = 0x0004,// �\���L�[��
	PAD_RIGHT = 0x0008,// �\���L�[�E
	PAD_START = 0x0010,// Start�{�^��
	PAD_BACK = 0x0020,// Back�{�^��
	PAD_LSTICK = 0x0040,// L�X�e�B�b�N��������
	PAD_RSTICK = 0x0080,// R�X�e�B�b�N��������
	PAD_LB = 0x0100,// LB�{�^��
	PAD_RB = 0x0200,// RB�{�^��
	PAD_A = 0x1000,// A�{�^��
	PAD_B = 0x2000,// B�{�^��
	PAD_X = 0x4000,// X�{�^��
	PAD_Y = 0x8000,// �x�{�^��
};

class Pad
{
private:
	// �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region �\����

#pragma endregion

#pragma region �����o�ϐ�
private:
	XINPUT_STATE state_ = {};
	XINPUT_STATE oldState_ = {};
#pragma endregion

#pragma region �����o�֐�
public:
	// �C���X�^���X�擾
	static Pad* GetInstance();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �w�肵���{�^���̓��͏�Ԃ��擾
	bool GetPushButton(BUTTON button) { return state_.Gamepad.wButtons & button; }

	// �w�肵���{�^�����������u�Ԃ��ǂ���
	bool GetTriggerButton(BUTTON button) { return (state_.Gamepad.wButtons & button) && !(oldState_.Gamepad.wButtons & button); }

	bool GetReleasButton(BUTTON button) { return !(state_.Gamepad.wButtons & button) && (oldState_.Gamepad.wButtons & button); }

	// L�g���K�[�̌��݂̒l��Ԃ�(�|���Γ|���قǒl���傫���Ȃ�0 ~ 255)
	uint8_t GetLTrigger() { return state_.Gamepad.bLeftTrigger; }

	// R�g���K�[�̌��݂̒l��Ԃ�(�|���Γ|���قǒl���傫���Ȃ�0 ~ 255)
	uint8_t GetRTrigger() { return state_.Gamepad.bLeftTrigger; }

	// ���X�e�B�b�N�̉���(X)�Əc��(Y)��Ԃ�(-1.0f ~ 1.0f)
	Vector2 GetLStick(int16_t deadZone = 5000);

	// �E�X�e�B�b�N�̉���(X)�Əc��(Y)��Ԃ�(-1.0f ~ 1.0f)
	Vector2 GetRStick(int16_t deadZone = 5000);
private:
	// �R���X�g���N�^
	Pad();

	// �f�X�g���N�^
	~Pad();
#pragma endregion

#pragma region ����֐�
	// �֎~
	Pad(const Pad&) = delete;				// �R�s�[�R���X�g���N�^�֎~
	Pad& operator = (const Pad&) = delete;	// �R�s�[������Z�q�֎~
#pragma endregion
};