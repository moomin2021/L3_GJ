#pragma once
#include <Windows.h>
#include <stdint.h>

class WinAPI {
#pragma region �����o�ϐ�
private:
	// �E�B���h�E�T�C�Y
	uint16_t winWidth_;	// ����
	uint16_t winHeight_;// �c��

	WNDCLASSEX	winClass_;	// �E�B���h�E�N���X
	RECT		winRect_;	// �E�B���h�E�T�C�Y
	HWND		hwnd_;		// �E�B���h�E�n���h��(�I�u�W�F�N�g)
	MSG			msg_;		// ���b�Z�[�W
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns> �C���X�^���X </returns>
	static WinAPI* GetInstance();

	/// <summary>
	/// ����������
	/// </summary>
	void Initialize(uint16_t winWidth, uint16_t winHeight);

	/// <summary>
	/// �E�B���h�E����郁�b�Z�[�W�����Ă��邩
	/// </summary>
	/// <returns> �^�U���� </returns>
	bool IsEndMessage();

#pragma region �Q�b�^�[�֐�
	/// <summary>
	/// �E�B���h�E�T�C�Y�̉������擾
	/// </summary>
	/// <returns> �E�B���h�E�T�C�Y�̉��� </returns>
	inline const uint16_t& GetWidth() { return winWidth_; }

	/// <summary>
	/// �E�B���h�E�T�C�Y�̏c�����擾
	/// </summary>
	/// <returns> �E�B���h�E�T�C�Y�̏c�� </returns>
	inline const uint16_t& GetHeight() { return winHeight_; }

	/// <summary>
	/// �E�B���h�E�N���X���擾
	/// </summary>
	/// <returns> �E�B���h�E�N���X </returns>
	inline const WNDCLASSEX& GetWinClass() { return winClass_; }

	/// <summary>
	/// �E�B���h�E�n���h�����擾
	/// </summary>
	/// <returns> �E�B���h�E�n���h�� </returns>
	inline const HWND& GetHWND() { return hwnd_; }

#pragma endregion

private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	WinAPI();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~WinAPI();

	/// <summary>
	/// �E�B���h�E�I�u�W�F�N�g�쐬
	/// </summary>
	void CreateWindowObj();

#pragma endregion

#pragma region ����֐�
	// �֎~
	WinAPI(const WinAPI&) = delete;				// �R�s�[�R���X�g���N�^�֎~
	WinAPI& operator = (const WinAPI&) = delete;// �R�s�[������Z�q�֎~
#pragma endregion
};