#include "PointCollider.h"

PointCollider::PointCollider(Vector2 pos)
{
	// �`��^�C�v�����ɐݒ�
	shapeType_ = SHAPE_POINT;

	// ���W��ݒ�
	Point::pos = pos;
}

void PointCollider::Update()
{
	// �Փ˃t���O��������
	isHit_ = false;
}