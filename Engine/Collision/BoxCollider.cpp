#include "BoxCollider.h"

BoxCollider::BoxCollider(const Vector2& offset, const Vector2& radius)
{
	// �`��^�C�v�����ɐݒ�
	shapeType_ = SHAPE_BOX;

	// �I�t�Z�b�g��ݒ�
	offset_ = offset;

	// ���a��ݒ�
	Box::radius = radius;
}

void BoxCollider::Update()
{
	// �Փ˃t���O��������
	isHit_ = false;

	// �X�v���C�g��R�Â��Ă�����
	if (sprite_) {
		Box::center = offset_ + sprite_->GetPosition();
	}

	else {
		Box::center = offset_;
	}
}