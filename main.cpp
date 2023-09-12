#pragma region ���N���X�ǂݍ���(�܂߂�)
#include "WinAPI.h"// -> WinAPI
#include "DX12Cmd.h"// -> DirectX12
#include "Texture.h"// -> Texture
#include "Key.h"// -> �L�[�{�[�h����
#include "Mouse.h"// -> �}�E�X�L�[�{�[�h����
#include "Pad.h"
#include "Object3D.h"// -> �I�u�W�F�N�g3D�N���X
#include "Sound.h"
#include "ImGuiManager.h"
#pragma endregion

#include "SceneManager.h"

// --Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)-- //
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// WinAPI�̏�����
	WinAPI::GetInstance()->Initialize(1856, 960);

	// DirectX12�̏�����
	DX12Cmd::GetInstance()->Initialize();

	// --�e�N�X�`���N���X-- //
	Texture::GetInstance()->Initialize();

	// �T�E���h�N���X
	Sound::GetInstance()->Initialize();

	// ImGui�N���X
	ImGuiManager::GetInstance()->Initialize();

	// --�Q�[�����[�v-- //
	while (true) {
		// --�I�����b�Z�[�W�����Ă����烋�[�v�I��-- //
		if (WinAPI::GetInstance()->IsEndMessage() == true) break;

		// �L�[�{�[�h���͍X�V����
		Key::GetInstance()->Update();

		// �}�E�X���͍X�V����
		Mouse::GetInstance()->Update();

		// �p�b�h���͍X�V
		Pad::GetInstance()->Update();

		// �T�E���h�X�V����
		Sound::GetInstance()->Update();

		// ImGui�`���t�J�n
		//ImGuiManager::GetInstance()->Begin();

		// �V�[���Ǘ��N���X�X�V����
		SceneManager::GetInstance()->Update();

		// ImGui�`���t�I��
		//ImGuiManager::GetInstance()->End();

		// �V�[���Ǘ��N���X�`�揈��
		SceneManager::GetInstance()->Draw();
	
		// ImGui�`�揈��
		//ImGuiManager::GetInstance()->Draw();

		SceneManager::GetInstance()->Post();
	}

	return 0;
}