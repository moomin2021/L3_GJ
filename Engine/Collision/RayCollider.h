#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

class RayCollider : public BaseCollider, public Ray
{
#pragma region �����o�ϐ�
private:
	// �I�u�W�F�N�g���S����̃I�t�Z�b�g
	Vector3 offset_ = { 0.0f, 0.0f, 0.0f };

	// �Փ˂����Ƃ��̏��
	Vector3 inter_ = { 0.0f, 0.0f, 0.0f };// ��_
	float minDistance_ = FLT_MAX;// ��_�܂ł̋���
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	RayCollider(Vector3 offset = { 0.0f, 0.0f, 0.0f }, Vector3 dir = { 0.0f, 0.0f, 0.0f });

	// �X�V����
	void Update() override;
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	// �I�t�Z�b�g��ݒ�
	inline void SetOffSet(const Vector3& offset) { offset_ = offset; }

	// ������ݒ�
	inline void SetDir(const Vector3& dir) { Ray::dir = dir; }

	// ��_��ݒ�
	inline void SetInter(const Vector3& inter) { inter_ = inter; }

	// ��_�܂ł̋�����ݒ�
	inline void SetDistance(float distance) { minDistance_ = distance; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	// ��_���擾
	inline const Vector3& GetInter() { return inter_; }

	// ��_�܂ł̋������擾
	inline float GetDistance() { return minDistance_; }
#pragma endregion
};