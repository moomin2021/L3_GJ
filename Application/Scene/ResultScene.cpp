#include "ResultScene.h"
#include "PipelineManager.h"
#include "Texture.h"

ResultScene::ResultScene() {}

ResultScene::~ResultScene() {}

void ResultScene::Initialize()
{
#pragma region インスタンス取得
	pad_ = Pad::GetInstance();
#pragma endregion

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	Sprite::SetCamera(camera_.get());
#pragma endregion

#pragma region スプライト
	sBackGround_ = std::make_unique<Sprite>();
	sBackGround_->SetSize({ 1856.0f, 960.0f });
#pragma endregion

#pragma region 画像ハンドル
	hBackGround_ = LoadTexture("Resources/background.png");
#pragma endregion
}

void ResultScene::Update()
{
	// 衝突時処理
	OnCollision();

	// 行列更新処理
	MatUpdate();
}

void ResultScene::Draw()
{
	// スプライト
	PipelineManager::PreDraw("Sprite");

	// 背景
	sBackGround_->Draw(hBackGround_);
}

void ResultScene::OnCollision()
{

}

void ResultScene::MatUpdate()
{
	// カメラ
	camera_->Update();

	// 背景
	sBackGround_->MatUpdate();
}