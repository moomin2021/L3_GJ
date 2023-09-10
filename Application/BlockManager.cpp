#include "BlockManager.h"
#include "CollisionManager2D.h"

#include <imgui_impl_DX12.h>

BlockManager* BlockManager::GetInstance()
{
	// インスタンス生成
	static BlockManager inst;

	// インスタンスを返す
	return &inst;
}

void BlockManager::Initialize()
{
	// ブロックにコリジョンマネージャーを設定
	Block::SetColMgr2D(CollisionManager2D::GetInstance());
}

void BlockManager::Update()
{
	// ブロックの生存フラグが[OFF]なら消す
	for (auto it = blocks_.begin(); it != blocks_.end();) {
		// ピースの更新
		(*it)->Update();

		// ピースの生存フラグが[OFF]になったら弾を削除
		if ((*it)->GetIsAlive() == false) it = blocks_.erase(it);
		else ++it;
	}

	ImGui::Text("BlockCount = %d", blocks_.size());
}

void BlockManager::Draw()
{
	// ブロック
	for (auto& it : blocks_) {
		it->Draw();
	}
}

void BlockManager::OnCollision()
{
	// ブロック
	for (auto& it : blocks_) {
		it->OnCollision();
	}
}

void BlockManager::MatUpdate()
{
	// ブロック
	for (auto& it : blocks_) {
		it->MatUpdate();
	}
}

Block* BlockManager::CreateBlock(const Vector2& offset, Piece* piece)
{
	// ブロック生成
	blocks_.emplace_back(std::make_unique<Block>());
	blocks_.back()->Initialize();
	blocks_.back()->SetPiece(piece);
	blocks_.back()->SetOffset(offset);

	// ブロックのポインタを返す
	return blocks_.back().get();
}

BlockManager::BlockManager() {}

BlockManager::~BlockManager() {}