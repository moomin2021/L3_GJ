#pragma once
#include "BaseCollider2D.h"
#include "CollisionPrimitive.h"

class PointCollider : public BaseCollider2D, public Point
{
#pragma region メンバ変数
private:

#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	PointCollider(Vector2 pos = {0.0f, 0.0f});

	// 更新処理
	void Update() override;
#pragma endregion

#pragma region セッター関数
public:
	// 座標を設定
	inline void SetPosition(const Vector2& pos) { Point::pos = pos; }
#pragma endregion

#pragma region ゲッター関数
	// 座標を取得
	inline const Vector2& GetPosition() { return Point::pos; }
#pragma endregion
};