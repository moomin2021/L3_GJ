#include "GameScene.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"
#include "Sound.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "ImGuiManager.h"
#include"WinAPI.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_DX12.h>

GameScene::GameScene() :
	key_(nullptr)
{
}

GameScene::~GameScene() {}

void GameScene::Initialize()
{
#pragma region インスタンス取得
	key_ = Key::GetInstance();				// キーボード入力
	blockMgr_ = BlockManager::GetInstance();// ブロックマネージャー
	blockMgr_->Initialize();
#pragma endregion

	// カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 5.0f, -20.0f });

	// カメラセット
	Sprite::SetCamera(camera_.get());
	ParticleEmitter::SetCamera(camera_.get());

	uint16_t blockTex = Texture::GetInstance()->LoadTexture("Resources/piece.png");
	uint16_t cannonTex = Texture::GetInstance()->LoadTexture("Resources/piece_cannon.png");
	uint16_t playerTex = Texture::GetInstance()->LoadTexture("Resources/player.png");

	// プレイヤー
	player = std::make_unique<Player>();
	player->Initialize();

	Block::SetPlayer(player.get());

	// ボス
	boss_ = std::make_unique<Boss>();
	boss_->Initialize();

	//背景
	backGroundTexture= Texture::GetInstance()->LoadTexture("Resources/backGround.png");
	backGroundSprite = std::make_unique<Sprite>();
	backGroundSprite->SetSize({ (float)WinAPI::GetInstance()->GetWidth(),(float)WinAPI::GetInstance()->GetHeight() });
	backGroundSprite->MatUpdate();
}

void GameScene::Update()
{
	// プレイヤー
	player->Update();

	// ボス
	boss_->Update();

	// ブロックマネージャー
	blockMgr_->Update();

	// ピースの生存フラグが[OFF]なら消す
	for (auto it = pieces_.begin(); it != pieces_.end();) {
		// ピースの更新
		(*it)->Update();

		// ピースの生存フラグが[OFF]になったら弾を削除
		if ((*it)->GetIsAlive() == false) it = pieces_.erase(it);
		else ++it;
	}

	if (ImGui::Button("add piece")) {
		CreatePiece();
	}

	// 衝突時処理
	OnCollision();

	// オブジェクト更新処理
	MatUpdate();

	ImGui::Text("PieceCount = %d", pieces_.size());
}

void GameScene::Draw()
{
	// 背景スプライト
	PipelineManager::PreDraw("Sprite");

	// 背景
	backGroundSprite->Draw(backGroundTexture);

	// 前景スプライト
	PipelineManager::PreDraw("Sprite");

	// プレイヤー
	player->Draw();

	// ボス
	boss_->Draw();

	// ブロックマネージャー
	blockMgr_->Draw();
}

void GameScene::MatUpdate()
{
	// カメラの更新
	camera_->Update();

	// ボス
	boss_->MatUpdate();

	// プレイヤー
	player->MatUpdate();

	// ブロックマネージャー
	blockMgr_->MatUpdate();
}

void GameScene::OnCollision()
{
	// 全判定チェック
	CollisionManager2D::GetInstance()->CheckAllCollision();

	// ボス
	boss_->OnCollision();

	// ブロックマネージャー
	blockMgr_->OnCollision();

	for (auto& it : pieces_) {
		it->OnCollision();
	}

}

void GameScene::CreatePiece()
{
	// ピース生成
	pieces_.emplace_back(std::make_unique<Piece>());
	pieces_.back()->Initialize(Vector2{2100.0f, 540.0f});
	pieces_.back()->AddBlock(blockMgr_->CreateBlock(Vector2{ 0.0f, 0.0f }, pieces_.back().get()));
	pieces_.back()->AddBlock(blockMgr_->CreateBlock(Vector2{ -1.0f, 0.0f }, pieces_.back().get()));
	pieces_.back()->AddBlock(blockMgr_->CreateBlock(Vector2{ 0.0f, -1.0f }, pieces_.back().get()));
	pieces_.back()->AddBlock(blockMgr_->CreateBlock(Vector2{ 1.0f, 0.0f }, pieces_.back().get()));
}