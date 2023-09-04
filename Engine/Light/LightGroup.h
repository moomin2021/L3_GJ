#pragma once
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"

#include <d3d12.h>
#include <wrl.h>

class LightGroup {
private:
	// �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

#pragma region �萔�����o�ϐ�
	// �e���C�g���
	static const uint16_t DIR_LIGHT_LIMIT = 1;
	static const uint16_t POINT_LIGHT_LIMIT = 10;
	static const uint16_t SPOT_LIGHT_LIMIT = 10;
	static const uint16_t CIRCLE_SHADOW_LIMIT = 1;
#pragma endregion

#pragma region �\����
public:
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		Vector3 ambientColor;// �����̐F
		float pad1;
		DirectionalLight::ConstBufferData dirLights[DIR_LIGHT_LIMIT];		// ���s�����p
		PointLight::ConstBufferData pointLights[POINT_LIGHT_LIMIT];			// �_�����p
		SpotLight::ConstBufferData spotLights[SPOT_LIGHT_LIMIT];			// �X�|�b�g���C�g�p
		CircleShadow::ConstBufferData circleShadows[CIRCLE_SHADOW_LIMIT];	// �ۉe�p
	};
#pragma endregion

#pragma region �����o�ϐ�
private:
	// �萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff_;// ���C�g�f�[�^

	// �����̐F
	Vector3 ambientColor_;
	
	// ���C�g�f�[�^
	std::vector<DirectionalLight*> dirLights_;	// ���s�����̔z��
	std::vector<PointLight*> pointLights_;		// �_�����̔z��
	std::vector<SpotLight*> spotLights_;			// �X�|�b�g���C�g�p
	std::vector<CircleShadow*> circleShadows_;	// �ۉe�̔z��

	// �_�[�e�B�t���O
	bool dirty_;
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	LightGroup();

	/// <summary>
	/// ���C�g�`��
	/// </summary>
	void Draw();

	/// <summary>
	/// ���s�����ǉ�
	/// </summary>
	void AddDirLight(DirectionalLight* light);

	/// <summary>
	/// �|�C���g���C�g�ǉ�
	/// </summary>
	void AddPointLight(PointLight* light);

	/// <summary>
	/// �X�|�b�g���C�g�ǉ�
	/// </summary>
	void AddSpotLight(SpotLight* light);

	/// <summary>
	/// �ۉe�ǉ�
	/// </summary>
	void AddCircleShadow(CircleShadow* shadow);

private:
	/// <summary>
	/// �萔�o�b�t�@�]��
	/// </summary>
	void TransferConstBuffer();
#pragma endregion
};