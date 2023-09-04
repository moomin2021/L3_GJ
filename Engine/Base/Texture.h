#pragma once
#include <d3d12.h>
#include <DirectXTex.h>
#include <wrl.h>
#include <string>
#include <map>

class Texture {
private:
	// �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region �����o�ϐ�
private:
	// �ǂݍ��񂾉�
	uint16_t loadCounter_ = 0;

	// �e�N�X�`���o�b�t�@
	std::map<const std::string, ComPtr<ID3D12Resource>> texBuff_;

	// �e�N�X�`���n���h��
	std::map<const std::string, uint16_t> texHandle_;

	// SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> srvHeap_ = nullptr;

	// SRV�q�[�v�̃n���h��
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle_{};

	std::vector<ID3D12Resource*> intermediateResources_;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �C���X�^���X�擾
	/// </summary>
	/// <returns> �C���X�^���X </returns>
	static Texture* GetInstance();

	/// <summary>
	/// ����������
	/// </summary>
	void Initialize();

	// �摜�C���[�W��ǂݎ��
	void LoadImageFile(const std::string filePath, DirectX::ScratchImage& scratchImage, DirectX::TexMetadata& metadata);

	// �e�N�X�`���o�b�t�@�𐶐�
	ID3D12Resource* CreateTextureResource(const DirectX::TexMetadata& metadata, D3D12_RESOURCE_DESC& texResourceDesc);

	// �e�N�X�`���o�b�t�@�Ƀf�[�^��]��
	ID3D12Resource* UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages);

	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);
	/// <summary>
	/// �e�N�X�`����ǂݍ��݁A�n���h�����擾����
	/// </summary>
	/// <param name="fileName"> �摜�t�@�C���� </param>
	/// <returns> �ǂݍ��񂾉摜�̃n���h�� </returns>
	uint16_t LoadTexture(const std::string fileName);

	void ReleaseIntermediateResources();

#pragma region �Q�b�^�[�֐�
	/// <summary>
	/// SRV�f�X�N���v�^�q�[�v�̎擾
	/// </summary>
	/// <returns> SRV�f�X�N���v�^�q�[�v </returns>
	ID3D12DescriptorHeap* GetSRVHeap() { return srvHeap_.Get(); }
#pragma endregion

private:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Texture();

	/// <summary>
	/// SRV�p�ŃX�N���v�^�q�[�v����
	/// </summary>
	void CreateDescriptorHeap();

#pragma endregion

#pragma region ����֐�
	// �֎~
	Texture(const Texture&) = delete;				// �R�s�[�R���X�g���N�^�֎~
	Texture& operator = (const Texture&) = delete;	// �R�s�[������Z�q�֎~
#pragma endregion
};

/// <summary>
/// �e�N�X�`����ǂݍ��݁A�n���h�����擾����
/// </summary>
/// <param name="fileName"> �摜�t�@�C���� </param>
/// <returns> �ǂݍ��񂾉摜�̃n���h�� </returns>
uint16_t LoadTexture(const std::string fileName);