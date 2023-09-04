#pragma once
#include "Vector3.h"

#include <d3d12.h>
#include <wrl.h>
#include <string>

class Material
{
#pragma region �G�C���A�X�e���v���[�g
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
#pragma endregion

#pragma region �\����
private:
	struct MaterialBuffer {
		Vector3 ambient;	// �A���r�G���g�W��
		float pad1;		// �p�f�B���O
		Vector3 diffuse;	// �f�B�t�F�[�Y�W��
		float pad2;		// �p�f�B���O
		Vector3 specular;// �X�y�L�����[�W��
		float alpha;	// �A���t�@
	};
#pragma endregion

#pragma region �����o�ϐ�
public:
	// �}�e���A���f�[�^
	std::string name_;	// �}�e���A����
	Vector3 ambient_;	// �A���r�G���g�e���x
	Vector3 diffuse_;	// �f�B�t���[�Y�e���x
	Vector3 specular_;	// �X�y�L�����[�e���x
	float alpha_;		// �A���t�@�l
	uint16_t texHandle_;// �e�N�X�`���n���h��
	ComPtr<ID3D12Resource> materialBuff_;	// �}�e���A���o�b�t�@
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	Material();

	// �`�揈��
	void Draw();

	/// <summary>
	/// �}�e���A���o�b�t�@�쐬
	/// </summary>
	void CreateMaterialBuff();
#pragma endregion
};