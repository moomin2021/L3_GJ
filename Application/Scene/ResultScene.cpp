#include "ResultScene.h"
#include "PipelineManager.h"
#include "Texture.h"
#include "WinAPI.h"

float ResultScene::clearTime_ = 0;

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

	Vector2 sTimesSize = { 80.0f, 80.0f };

	sClearTime_.resize(3);
	sClearTime_[0] = std::make_unique<Sprite>();
	sClearTime_[0]->SetPosition({ 576.0f, 480.0f });
	sClearTime_[0]->SetSize(sTimesSize);
	sClearTime_[0]->SetAnchorPoint({ 0.5f, 0.5f });

	sColon_ = std::make_unique<Sprite>();
	sColon_->SetPosition({ 636.0f, 480.0f });
	sColon_->SetSize(sTimesSize - Vector2{ 20.0f, 20.0f });
	sColon_->SetAnchorPoint({ 0.5f, 0.5f });

	sClearTime_[1] = std::make_unique<Sprite>();
	sClearTime_[1]->SetPosition({ 696.0f, 480.0f });
	sClearTime_[1]->SetSize(sTimesSize);
	sClearTime_[1]->SetAnchorPoint({ 0.5f, 0.5f });

	sClearTime_[2] = std::make_unique<Sprite>();
	sClearTime_[2]->SetPosition({ 756.0f, 480.0f });
	sClearTime_[2]->SetSize(sTimesSize);
	sClearTime_[2]->SetAnchorPoint({ 0.5f, 0.5f });

	sRank_ = std::make_unique<Sprite>();
	sRank_->SetPosition({ 1392.0f, 592.0f });
	sRank_->SetSize({ 214.0f, 214.0f });
	sRank_->SetAnchorPoint({ 0.5f, 0.5f });

	sTimesSize = { 60.0f, 60.0f };

	sNextRankTime_.resize(3);
	sNextRankTime_[0] = std::make_unique<Sprite>();
	sNextRankTime_[0]->SetPosition({ 1498.0f, 816.0f });
	sNextRankTime_[0]->SetSize(sTimesSize);
	sNextRankTime_[0]->SetAnchorPoint({ 0.5f, 0.5f });

	sNextRankTimeColon_ = std::make_unique<Sprite>();
	sNextRankTimeColon_->SetPosition({ 1538.0f, 816.0f });
	sNextRankTimeColon_->SetSize(sTimesSize - Vector2{ 20.0f, 20.0f });
	sNextRankTimeColon_->SetAnchorPoint({ 0.5f, 0.5f });

	sNextRankTime_[1] = std::make_unique<Sprite>();
	sNextRankTime_[1]->SetPosition({ 1578.0f, 816.0f });
	sNextRankTime_[1]->SetSize(sTimesSize);
	sNextRankTime_[1]->SetAnchorPoint({ 0.5f, 0.5f });

	sNextRankTime_[2] = std::make_unique<Sprite>();
	sNextRankTime_[2]->SetPosition({ 1618.0f, 816.0f });
	sNextRankTime_[2]->SetSize(sTimesSize);
	sNextRankTime_[2]->SetAnchorPoint({ 0.5f, 0.5f });
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
	hNumbers_.resize(10);
	hNumbers_[0] = LoadTexture("Resources/number_0.png");
	hNumbers_[1] = LoadTexture("Resources/number_1.png");
	hNumbers_[2] = LoadTexture("Resources/number_2.png");
	hNumbers_[3] = LoadTexture("Resources/number_3.png");
	hNumbers_[4] = LoadTexture("Resources/number_4.png");
	hNumbers_[5] = LoadTexture("Resources/number_5.png");
	hNumbers_[6] = LoadTexture("Resources/number_6.png");
	hNumbers_[7] = LoadTexture("Resources/number_7.png");
	hNumbers_[8] = LoadTexture("Resources/number_8.png");
	hNumbers_[9] = LoadTexture("Resources/number_9.png");
	hColon_ = LoadTexture("Resources/number_colon.png");

	hRank_.resize(5);
	hRank_[0] = LoadTexture("Resources/rank_s.png");
	hRank_[1] = LoadTexture("Resources/rank_a.png");
	hRank_[2] = LoadTexture("Resources/rank_b.png");
	hRank_[3] = LoadTexture("Resources/rank_c.png");
	hRank_[4] = LoadTexture("Resources/gameover.png");
#pragma endregion

#pragma region フェード
	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
#pragma endregion
	clearTimeHandle_.resize(3);
	clearTimeHandle_[0] = (uint16_t)clearTime_ / 60;
	clearTimeHandle_[1] = ((uint16_t)clearTime_ % 60) / 10;
	clearTimeHandle_[2] = ((uint16_t)clearTime_ % 60) % 10;

	hResultRank_ = hRank_[3];
	nextRankTime_ = clearTime_ - rankTime_[3];
	for (size_t i = 0; i < 4; i++) {
		if (rankTime_[i] >= clearTime_) {
			
			if (i == 0) {
				nextRankTime_ = 0.0;
			}

			else {
				nextRankTime_ = clearTime_ - rankTime_[i - 1];
			}
			hResultRank_ = hRank_[i];
			break;
		}
	}

	nextRankTimeHandle_.resize(3);
	nextRankTimeHandle_[0] = (uint16_t)nextRankTime_ / 60;
	nextRankTimeHandle_[1] = ((uint16_t)nextRankTime_ % 60) / 10;
	nextRankTimeHandle_[2] = ((uint16_t)nextRankTime_ % 60) % 10;
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
		if (selectNum_ == 0) fade_->ChangeScene(SCENE::GAME);
		if (selectNum_ == 1) fade_->ChangeScene(SCENE::TITLE);
	}

	// フェード
	fade_->Update();

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
	sClearTime_[0]->Draw(hNumbers_[clearTimeHandle_[0]]);
	sClearTime_[1]->Draw(hNumbers_[clearTimeHandle_[1]]);
	sClearTime_[2]->Draw(hNumbers_[clearTimeHandle_[2]]);
	sColon_->Draw(hColon_);
	sRank_->Draw(hResultRank_);
	sNextRankTime_[0]->Draw(hNumbers_[nextRankTimeHandle_[0]]);
	sNextRankTime_[1]->Draw(hNumbers_[nextRankTimeHandle_[1]]);
	sNextRankTime_[2]->Draw(hNumbers_[nextRankTimeHandle_[2]]);
	sNextRankTimeColon_->Draw(hColon_);

	// フェード
	fade_->Draw();
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
	for (auto& it : sClearTime_) it->MatUpdate();
	sColon_->MatUpdate();
	sRank_->MatUpdate();
	for (auto& it : sNextRankTime_) it->MatUpdate();
	sNextRankTimeColon_->MatUpdate();

	// フェード
	fade_->MatUpdate();
}