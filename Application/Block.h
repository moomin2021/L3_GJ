#pragma once
#include"Texture.h"
#include"Sprite.h"
#include<memory>
#include"BoxCollider.h"
#include"CollisionManager2D.h"

#include<list>

class Player;
class Piece;

class Block {
#pragma region メンバ変数
private:
	// インスタンス
	static CollisionManager2D* sColMgr2D_;
	static Player* sPlayer_;

	// スプライト
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// 画像ハンドル
	uint16_t handle_ = 0;

	// コライダー
	std::unique_ptr<BoxCollider> collider_ = nullptr;

	// 所属しているピース
	Piece* piece_ = nullptr;

	// オフセット
	Vector2 offset_ = { 0.0f, 0.0f };

	// 生存フラグ
	bool isAlive_ = true;

	// ピースに所属しているかフラグ
	bool isPiece_ = true;
#pragma endregion

#pragma region メンバ関数
public:
	// コンストラクタ
	Block();

	// デストラクタ
	~Block();

	// 初期化処理
	void Initialize();

	// 更新処理
	void Update();

	// 描画処理
	void Draw();

	// 衝突時処理
	void OnCollision();

	// 行列更新処理
	void MatUpdate();

	// ブロックの所属をプレイヤーに変える
	void SetAffChangePlayer();
#pragma endregion

#pragma region セッター関数
	// コリジョンマネージャー2Dを設定
	static void SetColMgr2D(CollisionManager2D* colMgr2D) { sColMgr2D_ = colMgr2D; }

	// プレイヤーを設定
	static void SetPlayer(Player* player) { sPlayer_ = player; }

	// 画像ハンドルを設定
	void SetHandle(uint16_t handle) { handle_ = handle; }

	// 座標を設定
	void SetPosition(const Vector2 pos) { sprite_->SetPosition(pos); }

	// ピースを設定
	void SetPiece(Piece* piece) { piece_ = piece; }

	// オフセット設定
	void SetOffset(const Vector2& offset) { offset_ = offset; }

	// 生存フラグを設定
	void SetIsAlive(bool frag) { isAlive_ = frag; }
#pragma endregion

#pragma region ゲッター関数
	// オフセットを取得
	const Vector2& GetOffset() { return offset_; }

	// 生存フラグを取得
	bool GetIsAlive() { return isAlive_; }
#pragma endregion
};