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
	// キーボード入力インスタンス取得
	key_ = Key::GetInstance();

	// カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 5.0f, -20.0f });

	// カメラセット
	Sprite::SetCamera(camera_.get());
	ParticleEmitter::SetCamera(camera_.get());

	uint16_t blockTex = Texture::GetInstance()->LoadTexture("Resources/piece.png");
	uint16_t cannonTex = Texture::GetInstance()->LoadTexture("Resources/piece_cannon.png");
	uint16_t playerTex = Texture::GetInstance()->LoadTexture("Resources/player.png");


	//ブロッククラス静的初期化
	Block::StaticInitialize(blockTex, cannonTex, { 32,32 });

	// プレイヤー
	player = std::make_unique<Player>();
	player->Initialize(playerTex,{96,(float)WinAPI::GetInstance()->GetHeight()/2});

	//プレイヤーとピース配列のセット
	Block::SetPlayer(player.get());
	Block::SetPiece(&Piece::pieces);


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

	//ピースの更新とボタンで生成
	for (size_t i = 0; i < Piece::pieces.size(); i++) {
		Piece::pieces[i]->Update();
	}

	ImGui::Text("piece size %d", Piece::pieces.size());

	//if (ImGui::Button("add piece")) {
	//	std::unique_ptr<Piece> newPiece = std::make_unique<Piece>();
	//	newPiece->Initialize();
	//	pieces.push_back(std::move(newPiece));
	//}

	Piece::CreatePiece();

	// 衝突時処理
	OnCollision();

	// オブジェクト更新処理
	MatUpdate();
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

	for (size_t i = 0; i < Piece::pieces.size(); i++) {
		Piece::pieces[i]->Draw();
	}

	// ボス
	boss_->Draw();
}

void GameScene::MatUpdate()
{
	// カメラの更新
	camera_->Update();

	// ボス
	boss_->MatUpdate();
}

void GameScene::OnCollision()
{
	// 全判定チェック
	CollisionManager2D::GetInstance()->CheckAllCollision();

	// ボス
	boss_->OnCollision();

	//判定
	for (size_t i = 0; i < Piece::pieces.size(); i++) {
		Piece::pieces[i]->OnCollision();
	}

}