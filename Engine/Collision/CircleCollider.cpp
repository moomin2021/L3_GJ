#include "CircleCollider.h"

CircleCollider::CircleCollider(Vector2 center, float radius)
{
	// 形状タイプを球に設定
	shapeType_ = SHAPE_CIRCLE;

	// 中心座標を設定
	Circle::center = center;

	// 半径を設定
	Circle::radius = radius;
}

void CircleCollider::Update()
{
	// 衝突フラグを初期化
	isHit_ = false;
}