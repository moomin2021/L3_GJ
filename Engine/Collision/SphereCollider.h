#pragma once
#include "BaseCollider.h"
#include "CollisionPrimitive.h"

class SphereCollider : public BaseCollider, public Sphere
{
#pragma region �����o�ϐ�
private:
	// �I�u�W�F�N�g���S����̃I�t�Z�b�g
	Vector3 offset_ = { 0.0f, 0.0f, 0.0f };

	// �Փ˂����Ƃ��̏��
	Vector3 inter_	= { 0.0f, 0.0f, 0.0f };// ��_
	Vector3 reject_	= { 0.0f, 0.0f, 0.0f };// �����o���x�N�g��
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	SphereCollider(Vector3 offset = { 0.0f, 0.0f, 0.0f }, float radius = 1.0f);

	// �X�V����
	void Update() override;
#pragma endregion

#pragma region �Z�b�^�[�֐�
public:
	// �I�t�Z�b�g��ݒ�
	inline void SetOffset(const Vector3& offset) { offset_ = offset; }

	// ��_��ݒ�
	inline void SetInter(const Vector3& inter) { inter_ = inter; }

	// ���a��ݒ�
	inline void SetRadius(float radius) { Sphere::radius = radius; }

	// �����o���x�N�g�������Z
	inline void AddReject(const Vector3& reject) { reject_ += reject; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	// ��_���擾
	inline const Vector3& GetInter() { return inter_; }

	// �����o���x�N�g�����擾
	inline const Vector3& GetReject() { return reject_; }
#pragma endregion
};