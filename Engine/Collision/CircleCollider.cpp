#include "CircleCollider.h"

CircleCollider::CircleCollider(Vector2 center, float radius)
{
	// �`��^�C�v�����ɐݒ�
	shapeType_ = SHAPE_CIRCLE;

	// ���S���W��ݒ�
	Circle::center = center;

	// ���a��ݒ�
	Circle::radius = radius;
}

void CircleCollider::Update()
{
	// �Փ˃t���O��������
	isHit_ = false;
}