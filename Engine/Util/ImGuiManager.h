#pragma once
#include <wrl.h>
#include <d3d12.h>

class ImGuiManager
{
private:
	// �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region �����o�ϐ�
private:
	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap_ = nullptr;
#pragma endregion

#pragma region �����o�֐�
public:
	// �C���X�^���X�擾
	static ImGuiManager* GetInstance();

	// ����������
	void Initialize();

	// ImGui��t�J�n
	void Begin();

	// ImGui��t�I��
	void End();

	// ��ʂւ̕`��
	void Draw();

private:
	// �R���X�g���N�^
	ImGuiManager();

	// �f�X�g���N�^
	~ImGuiManager();
#pragma endregion

#pragma region ����֐�
	// �֎~
	ImGuiManager(const ImGuiManager&) = delete;				// �R�s�[�R���X�g���N�^�֎~
	ImGuiManager& operator = (const ImGuiManager&) = delete;	// �R�s�[������Z�q�֎~
#pragma endregion
};