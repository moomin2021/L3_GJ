#pragma once
#include <d3dx12.h>
#include <wrl.h>

#include <vector>

// �V�F�[�_�[�̎��
enum ShaderType {
	VS,
	PS,
	GS
};

class PipelineObj
{
private:
	// �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	enum BLENDMODE {
		ALPHA,	// ������
		ADD,	// ���Z
		SUB,	// ���Z
		INV,	// ���]
	};

#pragma region �����o�ϐ�
private:
	// �V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3DBlob> vsBlob_ = nullptr;	// ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob_ = nullptr;	// �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> gsBlob_ = nullptr;	// �W�I���g���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob_;	// �G���[�I�u�W�F�N�g

	// ���_���C�A�E�g
	std::vector<D3D12_INPUT_ELEMENT_DESC> inputLayout_;

	// ���[�g�p�����[�^
	std::vector<CD3DX12_ROOT_PARAMETER> rootParams_;

	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature_;

	// �p�C�v���C���X�e�[�g
	ComPtr<ID3D12PipelineState> pipelineState_;

	// �f�X�N���v�^�����W
	std::vector <CD3DX12_DESCRIPTOR_RANGE> descRangeSRV_;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	PipelineObj();

	/// <summary>
	/// �V�F�[�_�[�ǂݍ���
	/// </summary>
	/// <param name="fileName"> �V�F�[�_�[�̃t�@�C���� </param>
	/// <param name="shaderType"> �V�F�[�_�[�̎�� </param>
	void LoadShader(std::string fileName, ShaderType shaderType);

	/// <summary>
	/// �C���v�b�g���C�A�E�g�ɐV�K�ݒ��ǉ�
	/// </summary>
	/// <param name="semanticName"> �Z�}���e�B�b�N�� </param>
	/// <param name="format"> ���\�[�X�f�[�^�`��(�v�f���ƃr�b�g����\��) </param>
	/// <param name="index"> �����Z�}���e�B�b�N������������Ƃ��g���C���f�b�N�X </param>
	void AddInputLayout(const char* semanticName, DXGI_FORMAT format, uint16_t index = 0);

	/// <summary>
	/// ���[�g�p�����[�^�쐬
	/// </summary>
	/// <param name="texRegisterNum"> �e�N�X�`�����W�X�^�� </param>
	/// <param name="constBuffNum"> �萔�o�b�t�@�̐� </param>
	void CreateRootParams(uint16_t texRegisterNum, uint16_t constBuffNum);

	/// <summary>
	/// �p�C�v���C���쐬
	/// </summary>
	/// <param name="renderTargetNum"> �����_�[�^�[�Q�b�g�̐� </param>
	void CreatePipeline(uint16_t renderTargetNum, BLENDMODE blendMode = ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE primitiveType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE, bool isDepth = true);

private:
	/// <summary>
	/// �V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	/// </summary>
	/// <param name="fileName"> �V�F�[�_�[�̃t�@�C���� </param>
	/// <param name="target"> �V�F�[�_�[���f���w�� </param>
	/// <param name="shaderBlob"> �V�F�[�_�[�I�u�W�F�N�g </param>
	void ShaderCompileFromFile(std::string fileName, LPCSTR target, ID3D10Blob** shaderBlob);
#pragma endregion

#pragma region �Q�b�^�[
public:
	/// <summary>
	/// �p�C�v���C���X�e�[�g�擾
	/// </summary>
	/// <returns> �p�C�v���C���X�e�[�g </returns>
	ID3D12PipelineState* GetPipelineState() { return pipelineState_.Get(); }

	/// <summary>
	/// ���[�g�V�O�l�`�����擾
	/// </summary>
	/// <returns> ���[�g�V�O�l�`�� </returns>
	ID3D12RootSignature* GetRootSignature() { return rootSignature_.Get(); }
#pragma endregion
};