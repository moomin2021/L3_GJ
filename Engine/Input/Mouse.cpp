#include "Mouse.h"
#include <cassert>
#define DIREXTINPUT_VERSION 0x0800// DirectInput�̃o�[�W�����w��

#include "WinAPI.h"

Mouse* Mouse::GetInstance()
{
	// �C���X�^���X����
	static Mouse inst;

	// �C���X�^���X��Ԃ�
	return &inst;
}

void Mouse::Update() {
	// �C���X�^���X�擾
	WinAPI* win = WinAPI::GetInstance();

	// �O�t���[���̃}�E�X�̓��͏�Ԃ�ۑ�
	oldMouse_ = nowMouse_;

	// �}�E�X�̓��͏�Ԃ��擾
	device_->GetDeviceState(sizeof(DIMOUSESTATE), &nowMouse_);

	// �}�E�X�f�o�C�X����J�n
	device_->Acquire();

	// �}�E�X�̍��W���擾
	GetCursorPos(&p_);
	ScreenToClient(FindWindowW(win->GetWinClass().lpszClassName, nullptr), &p_);
}

Mouse::Mouse() :
#pragma region ���������X�g
	// �f�o�C�X
	device_(nullptr),

	// �}�E�X�f�[�^
	nowMouse_{},// ���݂̃}�E�X�̓��͏��
	oldMouse_{},// �O�t���[���̃}�E�X�̓��͏��
	
	// �}�E�X�J�[�\�����W
	p_{}
#pragma endregion
{
	// �C���X�^���X�擾
	WinAPI* win = WinAPI::GetInstance();

	// �֐����s�̐��ۂ𔻕ʗp�̕ϐ�
	HRESULT result;

	// DirectInput�f�o�C�X����
	ComPtr<IDirectInput8> directInput = nullptr;
	result = DirectInput8Create(
		win->GetWinClass().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr
	);
	assert(SUCCEEDED(result));

	// �}�E�X�f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysMouse, &device_, NULL);
	assert(SUCCEEDED(result));

	// �}�E�X���̓f�[�^�̌`���̃Z�b�g
	result = device_->SetDataFormat(&c_dfDIMouse);// �W���`��
	assert(SUCCEEDED(result));

	// �r�����䃌�x���̃Z�b�g
	result = device_->SetCooperativeLevel(
		win->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	assert(SUCCEEDED(result));

	// �}�E�X�f�o�C�X����J�n
	device_->Acquire();

	// DirectInput�f�o�C�X�̉��
	directInput->Release();
}

Mouse::~Mouse()
{
	// �f�o�C�X����
	device_->Unacquire();

	// �f�o�C�X�̉��
	device_->Release();
}