#include "WinAPI.h"

#include <imgui_impl_win32.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

// �E�B���h�E�v���V�[�W���v���g�^�C�v�錾
// ���V�X�e�����b�Z�[�W���������邽�߂̊֐�
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	// ImGui�p�E�B���h�E�v���V�[�W���Ăяo��
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) return true;

	// ���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg)
	{
		// �E�B���h�E���j�����ꂽ
	case WM_DESTROY:
		// OS�ɑ΂��āA�A�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}

	// �W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

WinAPI* WinAPI::GetInstance()
{
	// �C���X�^���X����
	static WinAPI inst;

	// �C���X�^���X��Ԃ�
	return &inst;
}

void WinAPI::Initialize(uint16_t winWidth, uint16_t winHeight)
{
	// �E�B���h�E�T�C�Y�ۑ�
	winWidth_ = winWidth;
	winHeight_ = winHeight;

	// �E�B���h�E�N���X�̐ݒ�
	winClass_.cbSize = sizeof(WNDCLASSEX);// -> WNDCLASSEX�\���̂̃T�C�Y
	winClass_.lpfnWndProc = (WNDPROC)WindowProc; // �E�B���h�E�v���V�[�W����ݒ�(�A�h���X����)
	winClass_.lpszClassName = L"DirectXGame"; // �E�B���h�E�N���X��
	winClass_.hInstance = GetModuleHandle(nullptr); // �E�B���h�E�n���h��
	winClass_.hCursor = LoadCursor(NULL, IDC_ARROW); // �}�E�X�J�[�\���w��

	// �E�B���h�E�N���X��OS�ɓo�^����
	RegisterClassEx(&winClass_);

	// �E�B���h�E�T�C�Y{ X���W Y���W ���� �c�� }
	winRect_ = { 0, 0, winWidth, winHeight };

	// �����ŃT�C�Y��␳����
	AdjustWindowRect(&winRect_, WS_OVERLAPPEDWINDOW, false);

	// �E�B���h�E�I�u�W�F�N�g�̐���
	CreateWindowObj();

	// �E�B���h�E��\����Ԃɂ���
	ShowWindow(hwnd_, SW_SHOW);
}

bool WinAPI::IsEndMessage()
{
	// ���b�Z�[�W�����邩
	if (PeekMessage(&msg_, nullptr, 0, 0, PM_REMOVE))
	{
		// �L�[���̓��b�Z�[�W�̏���
		TranslateMessage(&msg_);

		// �v���V�[�W���Ƀ��b�Z�[�W�𑗂�
		DispatchMessage(&msg_);
	}

	// �~�{�^���ŏI�����b�Z�[�W��������u�^�v��Ԃ�
	if (msg_.message == WM_QUIT)
	{
		return true;
	}

	// �u�U�v��Ԃ�
	return false;
}

WinAPI::WinAPI() :
#pragma region ���������X�g
	// �E�B���h�E�T�C�Y
	winWidth_(0), winHeight_(0),

	// �E�B���h�E�ݒ�֘A
	winClass_{},// �E�B���h�E�N���X
	winRect_{},	// �E�B���h�E�T�C�Y
	hwnd_{},	// �E�B���h�E�n���h��(�I�u�W�F�N�g)
	msg_{}		// ���b�Z�[�W
#pragma endregion
{}

WinAPI::~WinAPI() {
	// �E�B���h�E�N���X�o�^����
	UnregisterClass(winClass_.lpszClassName, winClass_.hInstance);
}

void WinAPI::CreateWindowObj()
{
	// �E�B���h�E�I�u�W�F�N�g�̐���
	hwnd_ = CreateWindow(
		winClass_.lpszClassName,// ------> �N���X��
		L"�e���v�e���v�e�[�V����",// -------> �^�C�g���o�[�̕���
		WS_OVERLAPPEDWINDOW,// --> �W���I�ȃE�B���h�E�X�^�C��
		32,// --------> �\��X���W(OS�ɔC����)
		0,// --------> �\��Y���W(OS�ɔC����)
		winRect_.right - winRect_.left,// -> �E�B���h�E����
		winRect_.bottom - winRect_.top,// -> �E�B���h�E�c��
		nullptr,// --------------> �e�E�B���h�E�n���h��
		nullptr,// --------------> ���j���[�n���h��
		winClass_.hInstance,// ----------> �Ăяo���A�v���P�[�V�����n���h��
		nullptr);// -------------> �I�v�V����
}