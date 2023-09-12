#include "ResultScene.h"
#include "PipelineManager.h"
#include "Texture.h"
#include "WinAPI.h"

ResultScene::ResultScene() {}

ResultScene::~ResultScene() {}

void ResultScene::Initialize()
{
	Vector2 winCenter = { WinAPI::GetInstance()->GetWidth() / 2.0f, WinAPI::GetInstance()->GetHeight() / 2.0f };

#pragma region インスタンス取得
	pad_ = Pad::GetInstance();
	key_ = Key::GetInstance();
#pragma endregion

#pragma region カメラ
	camera_ = std::make_unique<Camera>();
	Sprite::SetCamera(camera_.get());
#pragma endregion

#pragma region スプライト
	sBackGround_ = std::make_unique<Sprite>();
	sBackGround_->SetSize({ 1856.0f, 960.0f });

	sFrame_ = std::make_unique<Sprite>();
	sFrame_->SetSize({ 1856.0f, 960.0f });

	sResultLogo_ = std::make_unique<Sprite>();
	sResultLogo_->SetPosition({winCenter.x, 256.0f});
	sResultLogo_->SetSize({704.0f, 176.0f});
	sResultLogo_->SetAnchorPoint({ 0.5f, 0.5f });

	sTimeText_ = std::make_unique<Sprite>();
	sTimeText_->SetPosition({320.0f, 480.0f});
	sTimeText_->SetSize({232.0f, 91.0f});
	sTimeText_->SetAnchorPoint({ 0.5f, 0.5f });

	sRetryText_ = std::make_unique<Sprite>();
	sRetryText_->SetPosition({ 464.0f, 624.0f });
	sRetryText_->SetSize({ 220.0f, 68.0f });
	sRetryText_->SetAnchorPoint({ 0.5f, 0.5f });

	sTitleText_ = std::make_unique<Sprite>();
	sTitleText_->SetPosition({ 464.0f, 752.0f });
	sTitleText_->SetSize({ 225.0f, 69.0f });
	sTitleText_->SetAnchorPoint({ 0.5f, 0.5f });

	sRankPiece_ = std::make_unique<Sprite>();
	sRankPiece_->SetPosition({ 1392.0f, 592.0f });
	sRankPiece_->SetSize({ 307.0f, 307.0f });
	sRankPiece_->SetAnchorPoint({ 0.5f, 0.5f });

	sRankText_ = std::make_unique<Sprite>();
	sRankText_->SetPosition({ 1296.0f, 432.0f });
	sRankText_->SetRotation(-15.0f);
	sRankText_->SetSize({ 322.0f, 89.0f });
	sRankText_->SetAnchorPoint({ 0.5f, 0.5f });

	sNextRankText_ = std::make_unique<Sprite>();
	sNextRankText_->SetPosition({ 1298.0f, 816.0f });
	sNextRankText_->SetSize({ 320.0f, 59.0f });
	sNextRankText_->SetAnchorPoint({ 0.5f, 0.5f });

	sResultSelectFrame_ = std::make_unique<Sprite>();
	sResultSelectFrame_->SetPosition({ 464.0f, 816.0f });
	sResultSelectFrame_->SetSize({ 306.0f, 112.0f });
	sResultSelectFrame_->SetAnchorPoint({ 0.5f, 0.5f });
#pragma endregion

#pragma region 画像ハンドル
	hBackGround_ = LoadTexture("Resources/background.png");
	hFrame_ = LoadTexture("Resources/title_frame.png");
	hResultLogo_ = LoadTexture("Resources/result_logo.png");
	hTimeText_ = LoadTexture("Resources/time_text.png");
	hRetryText_ = LoadTexture("Resources/retry_text.png");
	hTitleText_ = LoadTexture("Resources/title_text.png");
	hRankPiece_ = LoadTexture("Resources/rank_piece.png");
	hRankText_ = LoadTexture("Resources/rank_text.png");
	hNextRankText_ = LoadTexture("Resources/nextrank_text.png");
	hResultSelectFrame_ = LoadTexture("Resources/result_select_frame.png");
#pragma endregion
}

void ResultScene::Update()
{
	if (key_->TriggerKey(DIK_S)) {
		selectNum_++;
		if (selectNum_ > 1) selectNum_ = 0;
	}

	if (key_->TriggerKey(DIK_W)) {
		selectNum_--;
		if (selectNum_ < 0) selectNum_ = 1;
	}

	if (pad_->GetLStick().y <= -0.6f && oldLStickY > -0.6f) {
		selectNum_++;
		if (selectNum_ > 1) selectNum_ = 0;
	}

	if (pad_->GetLStick().y >= 0.6f && oldLStickY < 0.6f) {
		selectNum_--;
		if (selectNum_ < 0) selectNum_ = 1;
	}

	if (pad_->GetTriggerButton(PAD_A) || key_->TriggerKey(DIK_SPACE)) {

	}

	oldLStickY = pad_->GetLStick().y;

	sResultSelectFrame_->SetPosition(selectPos_[selectNum_]);

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
	sFrame_->Draw(hFrame_);
	sResultLogo_->Draw(hResultLogo_);
	sTimeText_->Draw(hTimeText_);
	sRetryText_->Draw(hRetryText_);
	sTitleText_->Draw(hTitleText_);
	sRankPiece_->Draw(hRankPiece_);
	sRankText_->Draw(hRankText_);
	sNextRankText_->Draw(hNextRankText_);
	sResultSelectFrame_->Draw(hResultSelectFrame_);
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
	sFrame_->MatUpdate();
	sResultLogo_->MatUpdate();
	sTimeText_->MatUpdate();
	sRetryText_->MatUpdate();
	sTitleText_->MatUpdate();
	sRankPiece_->MatUpdate();
	sRankText_->MatUpdate();
	sNextRankText_->MatUpdate();
	sResultSelectFrame_->MatUpdate();
}