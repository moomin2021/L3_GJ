#include "TitleScene.h"
#include "PipelineManager.h"
#include "Texture.h"
#include "Sound.h"

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
	sBackGround_->SetSize({ 1856.0f, 960.0f });

	sTitleFrame_ = std::make_unique<Sprite>();
	sTitleFrame_->SetSize({ 1856.0f, 960.0f });

	sTitleLogo_ = std::make_unique<Sprite>();
	sTitleLogo_->SetPosition({ 928.0f, 350.0f });
	sTitleLogo_->SetSize({ 1360.0f, 350.0f });
	sTitleLogo_->SetAnchorPoint({ 0.5f, 0.5f });

	sPressA_ = std::make_unique<Sprite>();
	sPressA_->SetPosition({ 928.0f, 750.0f });
	sPressA_->SetSize({314.0f, 64.0f});
	sPressA_->SetAnchorPoint({ 0.5f, 0.5f });
#pragma endregion

#pragma region 画像ハンドル
	hBackGround_ = LoadTexture("Resources/background.png");
	hTitleFrame_ = LoadTexture("Resources/title_frame.png");
	hTitleLogo_ = LoadTexture("Resources/title_logo.png");
	hPressA_ = LoadTexture("Resources/pressA.png");
#pragma endregion

	soundHandle_ = Sound::GetInstance()->LoadWave("Resources/Sound/a.wav");

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
}

void TitleScene::Update()
{
	// Aボタンを押したらゲームシーンに切り替える
	if (pad_->GetTriggerButton(PAD_A) || Key::GetInstance()->TriggerKey(DIK_SPACE)) {
		fade_->ChangeScene(SCENE::GAME);
	}

	fade_->Update();

	// 衝突時処理
	OnCollision();

	// 行列更新処理
	MatUpdate();
}

void TitleScene::Draw()
{
	// スプライト
	PipelineManager::PreDraw("Sprite");

	sBackGround_->Draw(hBackGround_);// 背景
	sTitleFrame_->Draw(hTitleFrame_);// フレーム
	sTitleLogo_->Draw(hTitleLogo_);// タイトルロゴ
	sPressA_->Draw(hPressA_);// プレスA

	fade_->Draw();
}

void TitleScene::OnCollision()
{
	
}

void TitleScene::MatUpdate()
{
	// カメラ
	camera_->Update();

	sBackGround_->MatUpdate();// 背景
	sTitleFrame_->MatUpdate();// フレーム
	sTitleLogo_->MatUpdate();// タイトルロゴ
	sPressA_->MatUpdate();// プレスA
	fade_->MatUpdate();
}