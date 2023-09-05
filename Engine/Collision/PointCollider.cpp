#include "PointCollider.h"

PointCollider::PointCollider(const Vector2& offset)
{
	// �`��^�C�v�����ɐݒ�
	shapeType_ = SHAPE_POINT;

	// �I�t�Z�b�g��ݒ�
	offset_ = offset;
}

void PointCollider::Update()
{
	// �Փ˃t���O��������
	isHit_ = false;

	// �X�v�^�C�g���R�Â����Ă�����
	if (sprite_) {
		Point::pos = offset_ + sprite_->GetPosition();
	}

	else {
		Point::pos = offset_;
	}
}