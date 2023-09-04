#pragma once
#include "Vector3.h"
#include "Vector3.h"

class DirectionalLight {
#pragma region �\����
public:
	// �萔�o�b�t�@�p�f�[�^�\��
	struct ConstBufferData {
		Vector3 lightVec;	// ���C�g�֕�����\��
		float pad1;
		Vector3 lightColor;	// ���C�g�̐F
		bool active;		// �L���t���O
	};
#pragma endregion

#pragma region �����o�ϐ�
private:
	// ���C�g��������
	Vector3 lightDir_ = { 0.0f, -1.0f, 0.0f };

	// ���C�g�F
	Vector3 lightColor_ = { 1.0f, 1.0f, 1.0f };

	// �L���t���O
	bool active_ = true;
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	/// <summary>
	/// ���C�g�̌���(XYZ)��ݒ�
	/// </summary>
	/// <param name="lightDir"> ���C�g�̌���(XYZ) </param>
	inline void SetLightDir(const Vector3& lightDir) { lightDir_ = Vector3Normalize(lightDir); }

	/// <summary>
	/// ���C�g�̐F(RGB)��ݒ�
	/// </summary>
	/// <param name="color"> ���C�g�̐F(RGB) </param>
	inline void SetLightColor(const Vector3& color) { lightColor_ = color; }

	/// <summary>
	/// ���C�g�̗L���t���O��ݒ�
	/// </summary>
	/// <param name="active"> �L���t���O </param>
	inline void SetActive(bool active) { active_ = active; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
public:
	/// <summary>
	/// ���C�g�̕���(XYZ)���擾
	/// </summary>
	/// <returns> ���C�g�̕���(XYZ) </returns>
	inline const Vector3& GetLightDir() { return lightDir_; }

	/// <summary>
	/// ���C�g�̐F(RGB)���擾
	/// </summary>
	/// <returns> ���C�g�̐F(RGB) </returns>
	inline const Vector3& GetLightColor() { return lightColor_; }

	/// <summary>
	/// �L���t���O���擾
	/// </summary>
	/// <returns> �L���t���O </returns>
	inline bool GetActive() { return active_; }
#pragma endregion
};