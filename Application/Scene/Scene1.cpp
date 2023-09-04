#include "Scene1.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"
#include "Sound.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "ImGuiManager.h"

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

	// プレイヤー
	player_ = std::make_unique<TestPlayer>();
	player_->Initialize();

	int a = 0;
	a++;

	// エネミー
	enemy_ = std::make_unique<TestEnemy>();
	enemy_->Initialize();
}

void Scene1::Update()
{
	// プレイヤー更新
	player_->Update();

	// エネミー更新
	enemy_->Update();

	// 衝突時処理
	OnCollision();

	// オブジェクト更新処理
	ObjUpdate();

	// カメラの更新
	camera_->Update();

	// 衝突判定
	CollisionManager::GetInstance()->CheckAllCollision();
}

void Scene1::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// プレイヤー描画
	player_->Draw();

	// エネミー描画
	enemy_->Draw();
}

void Scene1::ObjUpdate()
{
	player_->ObjUpdate();
	enemy_->ObjUpdate();
}

void Scene1::OnCollision()
{
	player_->OnCollision();
	enemy_->OnCollision();
}