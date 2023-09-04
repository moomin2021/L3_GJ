#pragma once
#include "BaseCollider2D.h"
#include "CollisionPrimitive.h"

class CircleCollider : public BaseCollider2D, public Circle
{
#pragma region メンバ変数
private:

#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	CircleCollider(Vector2 center = { 0.0f, 0.0f }, float radius = 1.0f);

	// 更新処理
	void Update() override;
#pragma endregion

#pragma region セッター関数
public:
	// 座標を設定
	inline void SetCenter(const Vector2& center) { Circle::center = center; }

	// 半径を設定
	inline void SetRadius(float radius) { Circle::radius = radius; }
#pragma endregion

#pragma region ゲッター関数
	// 座標を取得
	inline const Vector2& GetCenter() { return Circle::center; }

	// 半径を取得
	inline float GetRadius() { return Circle::radius; }
#pragma endregion
};