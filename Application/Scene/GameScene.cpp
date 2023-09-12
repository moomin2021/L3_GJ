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
	ParticleEmitter2D::SetCamera(camera_.get());

	uint16_t blockTex = Texture::GetInstance()->LoadTexture("Resources/piece.png");
	uint16_t cannonTex = Texture::GetInstance()->LoadTexture("Resources/piece_cannon.png");
	uint16_t playerTex = Texture::GetInstance()->LoadTexture("Resources/player.png");
	particleHandle_ = LoadTexture("Resources/effect2.png");


	//ブロッククラス静的初期化
	Block::StaticInitialize(blockTex, cannonTex,playerTex, { 32,32 });

	// プレイヤー
	Vector2 playerPos = { Block::GetBlockSize().x * 3.0f + (Block::GetBlockSize().x / 2.0f),
	Block::GetBlockSize().y * 15.0f - (Block::GetBlockSize().y / 2.0f) };


	player = std::make_unique<Player>();
	player->Initialize(playerTex,playerPos);
	PlayerBullet::SetColMgr2D(CollisionManager2D::GetInstance());

	//自機の弾のテクスチャの読み込みとセット
	PlayerBullet::SetHandle(LoadTexture("Resources/bullet_player.png"));

	//プレイヤーとピース配列のセット
	Block::SetPlayer(player.get());
	Block::SetPiece(&Piece::pieces);


	// ボス
	boss_ = std::make_unique<Boss>();
	boss_->Initialize();
	PlayerBullet::SetBoss(boss_.get());

	//背景
	backGroundTexture= Texture::GetInstance()->LoadTexture("Resources/backGround.png");
	backGroundSprite = std::make_unique<Sprite>();
	backGroundSprite->SetSize({ (float)WinAPI::GetInstance()->GetWidth(),(float)WinAPI::GetInstance()->GetHeight() });
	backGroundSprite->MatUpdate();

	// UIマネージャー
	uiMgr_ = std::make_unique<UIManager>();
	uiMgr_->Initialize();
}

void GameScene::Update()
{

	// プレイヤー
	player->Update();

	// ボス
	boss_->Update();

	Piece::ALlPieceUpdate();
	ImGui::Text("all block %d,", (int)Block::GetBlockCount());

	Block::AllBlockDeleteCheck();

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

	PipelineManager::PreDraw("Sprite");

	// UIマネージャー
	uiMgr_->Draw();
}

void GameScene::MatUpdate()
{
	// カメラの更新
	camera_->Update();

	//プレイヤー
	player->MatUpdate();

	// ボス
	boss_->MatUpdate();

	// UIマネージャー
	uiMgr_->MatUpdate();
}

void GameScene::OnCollision()
{
	// 全判定チェック
	CollisionManager2D::GetInstance()->CheckAllCollision();

	// ボス
	boss_->OnCollision();

	player->OnCollision();

	//判定
	for (size_t i = 0; i < Piece::pieces.size(); i++) {
		Piece::pieces[i]->OnCollision();
	}

}