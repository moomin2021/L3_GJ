#include "PointCollider.h"

PointCollider::PointCollider(Vector2 pos)
{
	// 形状タイプを球に設定
	shapeType_ = SHAPE_POINT;

	// 座標を設定
	Point::pos = pos;
}

void PointCollider::Update()
{
	// 衝突フラグを初期化
	isHit_ = false;
}