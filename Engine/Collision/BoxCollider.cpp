#include "BoxCollider.h"

BoxCollider::BoxCollider(Vector2 center, Vector2 radius)
{
	// �`��^�C�v�����ɐݒ�
	shapeType_ = SHAPE_BOX;

	// ���S���W��ݒ�
	Box::center = center;

	// ���a��ݒ�
	Box::radius = radius;
}

void BoxCollider::Update()
{
	// �Փ˃t���O��������
	isHit_ = false;
}