#include "RayCollider.h"

RayCollider::RayCollider(Vector3 offset, Vector3 dir)
{
	shapeType_ = SHAPE_RAY;

	offset_ = offset;

	Ray::dir = dir;
}

void RayCollider::Update()
{
	// �Փ˃t���O��������
	isHit_ = false;

	// �Փ˂����Ƃ��̏���������
	inter_ = { 0.0f, 0.0f, 0.0f };
	minDistance_ = FLT_MAX;

	// �I�u�W�F�N�g3D���R�Â����Ă�����
	if (object_) {
		// ���[���h�s�񂩂烏�[���h���W�𒊏o
		const Vector3 objPos = object_->GetPosition();
		Ray::start = objPos + offset_;
	}

	else {
		Ray::start = offset_;
	}
}