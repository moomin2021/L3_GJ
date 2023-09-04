#pragma once
#include "Matrix4.h"
#include "float4.h"
#include "Vector3.h"
#include "Model.h"
#include "Camera.h"
#include "LightGroup.h"

class Object3D {
private:
	// �G�C���A�X�e���v���[�g
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		Matrix4 viewProj;	// �r���[�v���W�F�N�V����
		Matrix4 world;		// ���[���h�s��
		Vector3 cameraPos;	// �J�������W(���[���h���W)
		float pad1;			// �p�f�B���O
		float4 color;		// �F(RGBA)
	};

#pragma region �����o�ϐ�
private:
	// �I�u�W�F�N�g�f�[�^
	Vector3 position_;	// �ʒu(XYZ)
	Vector3 rotation_;	// ��](XYZ)
	Vector3 scale_;		// �g�k(XYZ)
	float4 color_;		// �F(RGBA)

	// �I�u�W�F�N�g�f�[�^��ύX�������ǂ���
	bool hasChanget_;

	// ���[���h�s��
	Matrix4 matWorld_;

	// �萔�o�b�t�@
	ComPtr<ID3D12Resource>	constBuff_;	// �萔�o�b�t�@
	ConstBufferData*		constMap_;	// �}�b�s���O�����p

	// ���f��
	Model* model_;

	// �ÓI�����o�ϐ�
	static Camera*		sCamera_;		// �J����
	static LightGroup*	sLightGroup_;	// ���C�g
#pragma endregion

#pragma region �����o�֐�
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Object3D(Model* model);

	// �X�V����
	void Update();

	/// <summary>
	/// �`�揈��
	/// </summary>
	void Draw();
#pragma endregion

#pragma region �Z�b�^�[�֐�
	/// <summary>
	/// ���W(XYZ)��ݒ�
	/// </summary>
	/// <param name="position"> ���W(XYZ) </param>
	inline void SetPosition(const Vector3& position) { position_ = position, hasChanget_ = true; }

	/// <summary>
	/// ��](XYZ)��ݒ�
	/// </summary>
	/// <param name="rotation"> ��](XYZ) </param>
	inline void SetRotation(const Vector3& rotation) { rotation_ = rotation, hasChanget_ = true; }

	/// <summary>
	/// �g�k(XYZ)��ݒ�
	/// </summary>
	/// <param name="scale"> �g�k(XYZ) </param>
	inline void SetScale(const Vector3& scale) { scale_ = scale, hasChanget_ = true; }

	/// <summary>
	/// �F(RGBA)��ݒ�
	/// </summary>
	/// <param name="color"> �F(RGBA) </param>
	inline void SetColor(const float4& color) { color_ = color, hasChanget_ = true; }

	/// <summary>
	/// ���f����ݒ�
	/// </summary>
	/// <param name="model"> ���f�� </param>
	inline void SetModel(Model* model) { model_ = model; }

	/// <summary>
	/// �J������ݒ�
	/// </summary>
	/// <param name="camera"> �J���� </param>
	static inline void SetCamera(Camera* camera) { sCamera_ = camera; }

	/// <summary>
	/// ���C�g�O���[�v��ݒ�
	/// </summary>
	/// <param name="lightGroup"> ���C�g�O���[�v </param>
	static inline void SetLightGroup(LightGroup* lightGroup) { sLightGroup_ = lightGroup; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	/// <summary>
	/// ���W(XYZ)���擾
	/// </summary>
	/// <returns> ���W(XYZ) </returns>
	inline const Vector3& GetPosition() { return position_; }

	/// <summary>
	/// �g�k(XYZ)���擾
	/// </summary>
	/// <returns> �g�k(XYZ) </returns>
	inline const Vector3& GetScale() { return scale_; }

	/// <summary>
	/// ���[���h�s����擾
	/// </summary>
	/// <returns> ���[���h�s�� </returns>
	inline const Matrix4& GetMatWorld() { return matWorld_; }

	/// <summary>
	/// ���f�����擾
	/// </summary>
	/// <returns> ���f�� </returns>
	inline Model* GetModel() { return model_; }
#pragma endregion
};