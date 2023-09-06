#include "CircleCollider.h"

CircleCollider::CircleCollider(Vector2 offset, float radius)
{
	// �`��^�C�v�����ɐݒ�
	shapeType_ = SHAPE_CIRCLE;

	// �I�t�Z�b�g��ݒ�
	offset_ = offset;

	// ���a��ݒ�
	Circle::radius = radius;
}

void CircleCollider::Update()
{
	// �Փ˃t���O��������
	isHit_ = false;

	// �X�v���C�g���R�Â����Ă�����
	if (sprite_) {
		Circle::center = offset_ + sprite_->GetPosition();
	}

	else {
		Circle::center = offset_;
	}
}