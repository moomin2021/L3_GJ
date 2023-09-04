#include "BoxCollider.h"

BoxCollider::BoxCollider(Vector2 center, Vector2 radius)
{
	// 形状タイプを球に設定
	shapeType_ = SHAPE_BOX;

	// 中心座標を設定
	Box::center = center;

	// 半径を設定
	Box::radius = radius;
}

void BoxCollider::Update()
{
	// 衝突フラグを初期化
	isHit_ = false;
}