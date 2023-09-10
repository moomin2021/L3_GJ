#include "TitleScene.h"
#include "PipelineManager.h"
#include "Texture.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {}

void TitleScene::Initialize()
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
	sBackGround_->SetSize({ 1920.0f, 1080.0f });
#pragma endregion

#pragma region 画像ハンドル
	hBackGround_ = LoadTexture("Resources/background.png");
#pragma endregion
}

void TitleScene::Update()
{
	// 衝突時処理
	OnCollision();

	// 行列更新処理
	MatUpdate();
}

void TitleScene::Draw()
{
	// スプライト
	PipelineManager::PreDraw("Sprite");

	// 背景
	sBackGround_->Draw(hBackGround_);
}

void TitleScene::OnCollision()
{

}

void TitleScene::MatUpdate()
{
	// カメラ
	camera_->Update();

	// 背景
	sBackGround_->MatUpdate();
}