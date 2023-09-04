#include "Key.h"
#include "WinAPI.h"
#include <cassert>

Key* Key::GetInstance() {
	// �C���X�^���X����
	static Key inst;

	// �C���X�^���X��Ԃ�
	return &inst;
}

void Key::Update() {
	// �O�t���[���̃L�[�̏�Ԃ�ۑ�
	for (size_t i = 0; i < 256; i++) oldKeys_[i] = keys_[i];

	// �}�E�X�f�o�C�X����J�n
	device_->Acquire();

	// �S�L�[�̓��͏�Ԃ��擾����
	device_->GetDeviceState(static_cast<DWORD>(keys_.size()), keys_.data());
}

Key::Key() :
#pragma region ���������X�g
	// ���͏��
	keys_(256),		// ���݂̃L�[�{�[�h�̏��
	oldKeys_(256),	// �O�t���[���̃L�[�{�[�h�̏��

	// �f�o�C�X
	device_(nullptr)
#pragma endregion
{
	// �C���X�^���X�擾
	WinAPI* win = WinAPI::GetInstance();

	// �֐��������������ǂ����𔻕ʂ���p�ϐ�
	HRESULT result;

#pragma region DirectInput�̏�����

	ComPtr<IDirectInput8> directInput = nullptr;
	result = DirectInput8Create(
		win->GetWinClass().hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&directInput, nullptr
	);
	assert(SUCCEEDED(result));

#pragma endregion

#pragma region �f�o�C�X�̐���

	// �f�o�C�X�̐���
	result = directInput->CreateDevice(GUID_SysKeyboard, &device_, NULL);
	assert(SUCCEEDED(result));

	// �L�[�{�[�h���̓f�[�^�̌`���̃Z�b�g
	result = device_->SetDataFormat(&c_dfDIKeyboard);// �W���`��
	assert(SUCCEEDED(result));

	// �r�����䃌�x���̃Z�b�g
	result = device_->SetCooperativeLevel(
		win->GetHWND(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY
	);
	assert(SUCCEEDED(result));

	// �f�o�C�X����J�n
	device_->Acquire();

#pragma endregion
}

Key::~Key() {
	// �L�[�{�[�h�f�o�C�X�̐��䐧��
	device_->Unacquire();

	// �L�[�{�[�h�f�o�C�X�̉��
	device_->Release();
}