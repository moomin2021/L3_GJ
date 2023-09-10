#include "Piece.h"
#include"WinAPI.h"
#include"Util.h"
#include<imgui_impl_dx12.h>

Piece::Piece()
{
}

Piece::~Piece()
{
	// クリア
	blocks_.clear();
}

void Piece::Initialize(const Vector2& pos)
{
	// 初期座標設定
	position_ = pos;
}

void Piece::Update()
{
	// 座標更新
	position_.x -= moveSpd_;

	// ブロックの位置を修正
	for (auto& it : blocks_) {
		// ブロックのオフセットから座標を計算
		Vector2 newPos = position_ + it->GetOffset() * 32.0f;
		it->SetPosition(newPos);
	}

	// ピースが画面外に行ったら生存フラグを[OFF]にする
	if (position_.x <= -300.0f) {
		isAlive_ = false;
	}

	// ピースの生存フラグが[OFF]になったらブロックの生存フラグも[OFF]にする
	if (isAlive_ == false && isCol_ == false) {
		for (auto& it : blocks_) {
			it->SetIsAlive(false);
		}
	}
}

void Piece::Draw()
{
}

void Piece::OnCollision()
{
	// 衝突フラグが[ON]になったらブロックの所属をプレイヤーに変える
	if (isCol_) {
		isAlive_ = false;
		for (auto& it : blocks_) {
			it->SetAffChangePlayer();
		}
	}
}

void Piece::MatUpdate()
{
}