#pragma once
#include "CollisionShapeType.h"
#include "Sprite.h"

#include <stdint.h>

class BaseCollider2D {
public:
	friend class CollisionManager2D;

#pragma region メンバ変数
protected:
	// 形状タイプ
	CollisionShapeType shapeType_ = SHAPE_UNKNOWN;

	// 衝突判定属性
	uint16_t attribute_ = 0;

	// 任意で紐付けるスプライト
	Sprite* sprite_ = nullptr;

	// 衝突フラグ
	bool isHit_ = false;

	// 衝突したコライダー
	BaseCollider2D* hitCol_ = nullptr;

	// タグ
	uint16_t tag_ = 0;
#pragma endregion

#pragma region メンバ関数
public:
	// デストラクタ
	virtual ~BaseCollider2D() = default;

	// 更新処理
	virtual void Update() = 0;
#pragma endregion

#pragma region セッター関数
	// 形状タイプを設定
	inline void SetShapeType(CollisionShapeType shapeType) { shapeType_ = shapeType; }

	// 衝突判定属性を設定
	inline void SetAttribute(uint16_t attribute) { attribute_ = attribute; }

	// スプライトを設定
	inline void SetSprite(Sprite* sprite) { sprite_ = sprite; }

	// 衝突フラグを設定
	inline void SetIsHit(bool flag) { isHit_ = flag; }

	// 衝突したコライダーを設定
	inline void SetHitCollider(BaseCollider2D* collider) { hitCol_ = collider; }

	// タグを設定
	inline void SetTag(uint16_t tag) { tag_ = tag; }
#pragma endregion

#pragma region ゲッター関数
	// 形状タイプを取得
	inline CollisionShapeType GetShapeType() { return shapeType_; }

	// 衝突判定属性を取得
	inline uint16_t GetAttribute() { return attribute_; }

	// 衝突フラグを取得
	inline bool GetIsHit() { return isHit_; }

	// 衝突したコライダーを取得
	inline BaseCollider2D* GetHitCollider() { return hitCol_; }

	// 座標を取得
	Vector2 GetPosition() const;

	// タグを取得
	inline uint16_t GetTag() { return tag_; }
#pragma endregion
};