#pragma once

class CircleShadow {
#pragma region �\�z��
public:
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		Vector3 dir;				// ����
		Vector3 casterPos;			// �L���X�^�[���W
		float distCasterLight;	// �L���X�^�[�ƃ��C�g�̋���
		Vector3 atten;				// ���������W��
		float pad3;// �p�f�B���O
		Vector2 factorAngleCos;		// �����p�x
		bool active;				// �L���t���O
		float pad4;// �p�f�B���O
	};
#pragma endregion

#pragma region �����o�ϐ�
private:
	// ����(XYZ)
	Vector3 dir_ = { 1.0f, 0.0f, 0.0f };

	// �L���X�^�[���W(XYZ)
	Vector3 casterPos_ = { 0.0f, 0.0f, 0.0f };

	// �L���X�^�[�ƃ��C�g�̋���
	float distCasterLight_ = 100.0f;

	// ���������W��(XYZ)
	Vector3 atten_ = { 0.5f, 0.6f, 0.0f };

	// �����p�x(�J�n�p�x�A�I���p�x)
	Vector2 factorAngleCos_ = { 0.2f, 0.5f };

	// �L���t���O
	bool active_ = true;
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	/// <summary>
	/// ����(XYZ)��ݒ�
	/// </summary>
	/// <param name="dir"> ����(XYZ) </param>
	inline void SetDir(const Vector3& dir) { dir_ = Vector3Normalize(dir); }

	/// <summary>
	/// �L���X�^�[���W(XYZ)��ݒ�
	/// </summary>
	/// <param name="casterPos"> �L���X�^�[���W(XYZ) </param>
	inline void SetCasterPos(const Vector3& casterPos) { casterPos_ = casterPos; }

	/// <summary>
	/// �L���X�^�[�ƃ��C�g�̋�����ݒ�
	/// </summary>
	/// <param name="distCasterLight"> �L���X�^�[�ƃ��C�g�̋��� </param>
	inline void SetDistCasterLight(float distCasterLight) { distCasterLight_ = distCasterLight; }

	/// <summary>
	/// ���������W��(XYZ)��ݒ�
	/// </summary>
	/// <param name="atten"></param>
	inline void SetAtten(const Vector3& atten) { atten_ = atten; }

	/// <summary>
	/// �����p�x(�J�n�p�x�A�I���p�x)��ݒ�
	/// </summary>
	/// <param name="factorAngle"> �����p�x(�J�n�p�x, �I���p�x) </param>
	inline void SetFactorAngle(const Vector2& factorAngle) {
		factorAngleCos_.x = cosf(Util::Degree2Radian(factorAngle.x));
		factorAngleCos_.y = cosf(Util::Degree2Radian(factorAngle.y));
	}

	/// <summary>
	/// �L���t���O��ݒ�
	/// </summary>
	/// <param name="active"> �L���t���O </param>
	inline void SetActive(bool active) { active_ = active; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
public:
	/// <summary>
	/// ����(XYZ)���擾
	/// </summary>
	/// <returns></returns>
	inline const Vector3& GetDir() { return dir_; }

	/// <summary>
	/// �L���X�^�[�̍��W(XYZ)���擾
	/// </summary>
	/// <returns> �L���X�^�[�̍��W(XYZ) </returns>
	inline const Vector3& GetCasterPos() { return casterPos_; }

	/// <summary>
	/// �L���X�^�[�ƃ��C�g�̋������擾
	/// </summary>
	/// <returns> �L���X�^�[�ƃ��C�g�̋��� </returns>
	inline float GetDistCasterLight() { return distCasterLight_; }

	/// <summary>
	/// ���������W��(XYZ)���擾
	/// </summary>
	/// <returns> ���������W��(XYZ) </returns>
	inline const Vector3& GetAtten() { return atten_; }

	/// <summary>
	/// �����p�x(�J�n�p�x�A�I���p�x)���擾
	/// </summary>
	/// <returns> �����p�x(�J�n�p�x�A�I���p�x) </returns>
	inline const Vector2& GetFactorAngleCos() { return factorAngleCos_; }

	/// <summary>
	/// �L���t���O���擾
	/// </summary>
	/// <returns> �L���t���O </returns>
	inline bool GetActive() { return active_; }
#pragma endregion
};