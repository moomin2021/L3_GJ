#pragma once
#include "BaseCollider2D.h"
#include "CollisionPrimitive.h"

class BoxCollider : public BaseCollider2D, public Box
{
#pragma region メンバ変数
private:

#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	BoxCollider(Vector2 center = { 0.0f, 0.0f }, Vector2 radius = {0.0f, 0.0f});

	// 更新処理
	void Update() override;
#pragma endregion

#pragma region セッター関数
public:
	// 座標を設定
	inline void SetCenter(const Vector2& center) { Box::center = center; }

	// 半径を設定
	inline void SetRadius(const Vector2& radius) { Box::radius = radius; }
#pragma endregion

#pragma region ゲッター関数
	// 座標を取得
	inline const Vector2& GetCenter() { return Box::center; }

	// 半径を取得
	inline const Vector2& GetRadius() { return Box::radius; }
#pragma endregion
};