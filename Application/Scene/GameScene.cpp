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

GameScene::~GameScene() {
	sound_->Stop(gameBGM_);
}

void GameScene::Initialize()
{
	// キーボード入力インスタンス取得
	key_ = Key::GetInstance();
	sound_ = Sound::GetInstance();

#pragma region サウンド
	gameBGM_ = sound_->LoadWave("Resources/Sound/game_bgm.wav", 0.01f);

	// BGMを鳴らす
	sound_->Play(gameBGM_, true);
#pragma endregion

	// カメラ
	camera_ = std::make_unique<Camera>();

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

	psManager = std::make_unique<PieceManager>();
	psManager->Initialize();

	//プレイヤーとピース配列のセット
	Block::SetPlayer(player.get());
	Block::SetPiece(&psManager->pieces);


	// ボス
	boss_ = std::make_unique<Boss>();
	boss_->Initialize();
	PlayerBullet::SetBoss(boss_.get());
	BossBullet::SetColMgr2D(CollisionManager2D::GetInstance());

	//背景
	backGroundTexture= Texture::GetInstance()->LoadTexture("Resources/backGround.png");
	backGroundSprite = std::make_unique<Sprite>();
	backGroundSprite->SetSize({ (float)WinAPI::GetInstance()->GetWidth(),(float)WinAPI::GetInstance()->GetHeight() });
	backGroundSprite->MatUpdate();

	// UIマネージャー
	uiMgr_ = std::make_unique<UIManager>();
	uiMgr_->Initialize();
	UIManager::SetBoss(boss_.get());

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
}

void GameScene::Update()
{
	// プレイヤー
	player->Update();

	//自機が動いたら一度衝突判定をとる
	OnCollision();

	// ボス
	boss_->Update();

	// チェンジシーンフラグが[ON]になったら切り替える
	if (boss_->GetIsChangeScene()) {
		fade_->ChangeScene(SCENE::RESULT);
	}

	if (key_->TriggerKey(DIK_B)) {
		boss_->SubHP(50);
	}

//	Piece::ALlPieceUpdate();

	psManager->Update();

	//ImGui::Text("all block %d,", (int)Block::GetBlockCount());

	// UIマネージャー
	uiMgr_->Update();

	Block::AllBlockDeleteCheck();

	fade_->Update();

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

	for (size_t i = 0; i < psManager->pieces.size(); i++) {
		psManager->pieces[i]->Draw();
	}

	// ボス
	boss_->Draw();

	PipelineManager::PreDraw("Sprite");

	// UIマネージャー
	uiMgr_->Draw();

	//自機UI描画
	player->DrawUI();

	fade_->Draw();

	//next描画
	psManager->DrawNextPiece();
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

	// フェード
	fade_->MatUpdate();
}

void GameScene::OnCollision()
{
	// 全判定チェック
	CollisionManager2D::GetInstance()->CheckAllCollision();

	// ボス
	boss_->OnCollision();

	player->OnCollision();

	//判定
	for (size_t i = 0; i < psManager->pieces.size(); i++) {
		psManager->pieces[i]->OnCollision();
	}

}