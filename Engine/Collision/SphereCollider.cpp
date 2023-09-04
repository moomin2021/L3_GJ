#include "SphereCollider.h"

SphereCollider::SphereCollider(Vector3 offset, float radius)
{
	// �`��^�C�v�����ɐݒ�
	shapeType_ = SHAPE_SPHERE;

	// �I�t�Z�b�g��ݒ�
	offset_ = offset;

	// ���a��ݒ�
	Sphere::radius = radius;
}

void SphereCollider::Update()
{
	// �Փ˃t���O��������
	isHit_ = false;

	// �Փ˂����Ƃ��̏���������
	inter_ = { 0.0f, 0.0f, 0.0f };
	reject_ = { 0.0f, 0.0f, 0.0f };

	// �I�u�W�F�N�g3D���R�Â����Ă�����
	if (object_) {
		Sphere::center = object_->GetPosition() + offset_;
	}

	else {
		Sphere::center = offset_;
	}
}