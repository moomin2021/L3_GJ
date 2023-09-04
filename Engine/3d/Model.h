#pragma once
#include "Mesh.h"
#include "Material.h"

#include <vector>
#include <map>
#include <d3d12.h>
#include <wrl.h>

class Model {
#pragma region �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
#pragma endregion

#pragma region �����o�ϐ�
private:
	// ���b�V���f�[�^
	std::vector<Mesh> meshes_;

	// �}�e���A���f�[�^
	std::map<std::string, Material> materials_;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="fileName"> ���f���t�@�C���� </param>
	Model(std::string fileName);

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

private:
	/// <summary>
	/// ���f���ǂݍ���
	/// </summary>
	/// <param name="name"> �t�@�C���� </param>
	void LoadModel(std::string name);

	/// <summary>
	/// �}�e���A���ǂݍ���
	/// </summary>
	/// <param name="directoryPath"> �t�@�C���� </param>
	/// <param name="fileName"> �t�@�C���� </param>
	void LoadMaterial(const std::string& directoryPath, const std::string& fileName);
#pragma endregion

#pragma region �Q�b�^�[�֐�
public:
	/// <summary>
	/// ���b�V���R���e�i���擾
	/// </summary>
	/// <returns> ���b�V���R���e�i </returns>
	inline const std::vector<Mesh>& GetMesh() { return meshes_; }
#pragma endregion
};