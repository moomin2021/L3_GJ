#pragma once
#include "Vector2.h"
#include "Vector3.h"

#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <vector>

class Mesh
{
#pragma region �G�C���A�X�e���v���[�g
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
#pragma endregion

#pragma region �\����
public:
	// ���_�f�[�^
	struct Vertex {
		Vector3 pos;		// ���W(XYZ)
		Vector3 normal;	// �@��(XYZ)
		Vector2 uv;		// UV���W(XY)
	};
#pragma endregion

#pragma region �����o�ϐ�
private:
	// ���_�f�[�^
	std::vector<Vertex> vertices_;			// ���_�f�[�^
	D3D12_VERTEX_BUFFER_VIEW vertexView_;	// ���_�o�b�t�@�[�r���[
	ComPtr<ID3D12Resource> vertexBuff_;		// ���_�o�b�t�@

	// �C���f�b�N�X�f�[�^
	std::vector<uint16_t> indexes_;		// �C���f�b�N�X�f�[�^
	D3D12_INDEX_BUFFER_VIEW indexView_;	// �C���f�b�N�X�o�b�t�@�r���[
	ComPtr<ID3D12Resource> indexBuff_;	// �C���f�b�N�X�o�b�t�@

	// �K�p�����}�e���A���̖��O
	std::string materialName_;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Mesh();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���_�f�[�^��ǉ�
	/// </summary>
	/// <param name="vertex"> ���_�f�[�^ </param>
	void AddVertex(const Vertex& vertex) { vertices_.emplace_back(vertex); }

	/// <summary>
	/// �C���f�b�N�X�f�[�^��ǉ�
	/// </summary>
	/// <param name="index"> �C���f�b�N�X�f�[�^ </param>
	void AddIndex(uint16_t index) { indexes_.emplace_back(index); }

	/// <summary>
	/// ���_�o�b�t�@�쐬
	/// </summary>
	void CreateVertexBuff();

	/// <summary>
	/// �C���f�b�N�X�o�b�t�@�쐬
	/// </summary>
	void CreateIndexBuff();
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	/// <summary>
	/// �}�e���A���̖��O��ݒ�
	/// </summary>
	/// <param name="name"> �}�e���A���̖��O </param>
	void SetMaterialName(const std::string& name) { materialName_ = name; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	/// <summary>
	/// �}�e���A���̖��O���擾
	/// </summary>
	/// <returns> �}�e���A���̖��O </returns>
	const std::string& GetMaterialName() { return materialName_; }

	/// <summary>
	/// ���_�f�[�^�R���e�i���擾
	/// </summary>
	/// <returns> ���_�f�[�^ </returns>
	inline const std::vector<Vertex>& GetVertex() { return vertices_; }

	/// <summary>
	/// �C���f�b�N�X�f�[�^�R���e�i���擾
	/// </summary>
	/// <returns> �C���f�b�N�X�f�[�^�R���e�i </returns>
	inline const std::vector<uint16_t>& GetIndex() { return indexes_; }
#pragma endregion
};