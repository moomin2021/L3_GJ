#include "Scene1.h"
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

Scene1::Scene1() :
	key_(nullptr)
{
}

Scene1::~Scene1() {}

void Scene1::Initialize()
{
	// キーボード入力インスタンス取得
	key_ = Key::GetInstance();

	// カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 5.0f, -20.0f });

	// カメラセット
	Object3D::SetCamera(camera_.get());
	Sprite::SetCamera(camera_.get());
	ParticleEmitter::SetCamera(camera_.get());

	// ライトグループ生成
	lightGroup_ = std::make_unique<LightGroup>();

	// ライトグループセット
	Object3D::SetLightGroup(lightGroup_.get());

	// 平行光源生成
	dirLight_ = std::make_unique<DirectionalLight>();

	// 平行光源セット
	lightGroup_->AddDirLight(dirLight_.get());

	// サウンド読み込み＆再生
	bgmKey_ = Sound::LoadWave("Resources/Sound/a.wav");
	Sound::SetVolume(bgmKey_, 0.001f);
	Sound::Play(bgmKey_);

	uint16_t blockTex = Texture::GetInstance()->LoadTexture("Resources/piece.png");
	uint16_t cannonTex = Texture::GetInstance()->LoadTexture("Resources/piece_cannon.png");
	uint16_t playerTex = Texture::GetInstance()->LoadTexture("Resources/player.png");


	//ブロッククラス静的初期化
	Block::StaticInitialize(blockTex, cannonTex, { 32,32 });

	// プレイヤー
	player_ = std::make_unique<TestPlayer>();
	player_->Initialize();

	player = std::make_unique<Player>();
	player->Initialize(playerTex,{96,(float)WinAPI::GetInstance()->GetHeight()/2});

	// エネミー
	enemy_ = std::make_unique<TestEnemy>();
	enemy_->Initialize();

	//背景
	backGroundTexture= Texture::GetInstance()->LoadTexture("Resources/backGround.png");
	backGroundSprite = std::make_unique<Sprite>();
	backGroundSprite->SetSize({ (float)WinAPI::GetInstance()->GetWidth(),(float)WinAPI::GetInstance()->GetHeight() });
	backGroundSprite->MatUpdate();
}

void Scene1::Update()
{
	// プレイヤー更新
	player_->Update();

	player->Update();

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

	// エネミー更新
	enemy_->Update();

	// 衝突時処理
	OnCollision();

	// オブジェクト更新処理
	ObjUpdate();

	// カメラの更新
	camera_->Update();
}

void Scene1::Draw()
{
	
	PipelineManager::PreDraw("Object3D");

	// プレイヤー描画
	player_->Draw();

	// エネミー描画
	enemy_->Draw();

	PipelineManager::PreDraw("Sprite");

	player->Draw();

	for (size_t i = 0; i < Piece::pieces.size(); i++) {
		Piece::pieces[i]->Draw();
	}

	backGroundSprite->Draw(backGroundTexture);

}

void Scene1::ObjUpdate()
{
	player_->ObjUpdate();
	enemy_->ObjUpdate();
}

void Scene1::OnCollision()
{
	CollisionManager2D::GetInstance()->CheckAllCollision();

	player_->OnCollision();
	enemy_->OnCollision();

	//判定
	for (size_t i = 0; i < Piece::pieces.size(); i++) {
		Piece::pieces[i]->OnCollision();
	}

}