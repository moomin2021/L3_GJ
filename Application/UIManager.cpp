#include "UIManager.h"
#include "Texture.h"
#include "Boss.h"
#include "Util.h"
#include "ResultScene.h"

Boss* UIManager::sBoss_ = nullptr;

UIManager::UIManager() {}

UIManager::~UIManager() {}

void UIManager::Initialize()
{
#pragma region スプライト
	sTitleFrame_ = std::make_unique<Sprite>();
	sTitleFrame_->SetSize({ 1856.0f, 960.0f });

	sExp_ = std::make_unique<Sprite>();
	sExp_->SetPosition({ 352.0f, 912.0f });
	sExp_->SetSize({ 640.0f, 48.0f });
	sExp_->SetAnchorPoint({ 0.5f, 0.5f });

	sExpIn_ = std::make_unique<Sprite>();
	sExpIn_->SetPosition({ 36.0f, 892.0f });
	sExpIn_->SetSize({ 632.0f, 40.0f });

	sExpText_ = std::make_unique<Sprite>();
	sExpText_->SetPosition({ 68.0f, 892.0f });
	sExpText_->SetSize({ 120.0f, 48.0f });
	sExpText_->SetAnchorPoint({ 0.5f, 0.5f });
	sExpText_->SetRotation(-15.0f);

	sHpPlayer_ = std::make_unique<Sprite>();
	sHpPlayer_->SetPosition({ 288.0f, 45.0f });
	sHpPlayer_->SetSize({ 512.0f, 48.0f });
	sHpPlayer_->SetAnchorPoint({ 0.5f, 0.5f });

	sHpPlayerIn_ = std::make_unique<Sprite>();
	sHpPlayerIn_->SetPosition({ 36.0f, 25.0f });
	sHpPlayerIn_->SetSize({ 504.0f, 40.0f });

	sHpTextPlayer_ = std::make_unique<Sprite>();
	sHpTextPlayer_->SetPosition({ 58.0f, 25.0f });
	sHpTextPlayer_->SetSize({ 98.0f, 48.0f });
	sHpTextPlayer_->SetAnchorPoint({ 0.5f, 0.5f });
	sHpTextPlayer_->SetRotation(-15.0f);

	sLevelTextPlayer_ = std::make_unique<Sprite>();
	sLevelTextPlayer_->SetPosition({ 744.0f,912.0f });
	sLevelTextPlayer_->SetSize({ 80.0f,50.0f });
	sLevelTextPlayer_->SetAnchorPoint({ 0.5f,0.5f });

	sLevelNumberPlayer_ = std::make_unique<Sprite>();
	sLevelNumberPlayer_->SetPosition({ 816.0f,912.0f });
	sLevelNumberPlayer_->SetSize({ 56.0f,56.0f });
	sLevelNumberPlayer_->SetAnchorPoint({ 0.5f,0.5f });

	sHpBoss_ = std::make_unique<Sprite>();
	sHpBoss_->SetPosition({ 1424.0f, 912.0f });
	sHpBoss_->SetSize({ 800.0f, 48.0f });
	sHpBoss_->SetAnchorPoint({ 0.5f, 0.5f });

	sHpTextBoss_ = std::make_unique<Sprite>();
	sHpTextBoss_->SetPosition({ 1046.0f, 892.0f });
	sHpTextBoss_->SetSize({ 98.0f, 48.0f });
	sHpTextBoss_->SetAnchorPoint({ 0.5f, 0.5f });
	sHpTextBoss_->SetRotation(-15.0f);

	sNextText_ = std::make_unique<Sprite>();
	sNextText_->SetPosition({ 1800.0f, 90.0f });
	sNextText_->SetSize({ 100.0f, 32.0f });
	sNextText_->SetAnchorPoint({ 0.5f, 0.5f });
	sNextText_->SetRotation(-15.0f);

	Vector2 sTimesSize = { 60.0f, 60.0f };

	sTimes_.resize(3);
	sTimes_[0] = std::make_unique<Sprite>();
	sTimes_[0]->SetPosition({ 888.0f, 49.0f });
	sTimes_[0]->SetSize(sTimesSize);
	sTimes_[0]->SetAnchorPoint({ 0.5f, 0.5f });

	sColon_ = std::make_unique<Sprite>();
	sColon_->SetPosition({ 928.0f, 49.0f });
	sColon_->SetSize(sTimesSize - Vector2{ 20.0f, 20.0f });
	sColon_->SetAnchorPoint({ 0.5f, 0.5f });

	sTimes_[1] = std::make_unique<Sprite>();
	sTimes_[1]->SetPosition({ 968.0f, 49.0f });
	sTimes_[1]->SetSize(sTimesSize);
	sTimes_[1]->SetAnchorPoint({ 0.5f, 0.5f });

	sTimes_[2] = std::make_unique<Sprite>();
	sTimes_[2]->SetPosition({ 1012.0f, 49.0f });
	sTimes_[2]->SetSize(sTimesSize);
	sTimes_[2]->SetAnchorPoint({ 0.5f, 0.5f });
#pragma endregion

#pragma region 画像ハンドル
	hTitleFrame_ = LoadTexture("Resources/frame.png");
	hExpIn_ = LoadTexture("Resources/hp_player_in.png");
	hExp_ = LoadTexture("Resources/hp_player.png");
	hExpText_ = LoadTexture("Resources/exp_text.png");
	hHpPlayer_ = LoadTexture("Resources/hp_player.png");
	hHpPlayerIn_ = LoadTexture("Resources/hp_player_in.png");
	hHpTextPlayer_ = LoadTexture("Resources/hp_text_player.png");
	hLevelTextPlayer_ = LoadTexture("Resources/level_shadow.png");
	hHpBoss_ = LoadTexture("Resources/hp_boss.png");
	hHpTextBoss_ = LoadTexture("Resources/hp_text_boss.png");
	hNextText_ = LoadTexture("Resources/next_text.png");
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
#pragma endregion

	gameTimes_.resize(3);
	clearTime_.resize(3);
	startGameTime_ = Util::GetTimrMSec();
}

void UIManager::Update()
{
	if (isClear_ == false) {
		// 経過時間を計算
		float elapsedTime = (Util::GetTimrMSec() - startGameTime_) / 1000.0f;

		gameTimes_[0] = (uint16_t)elapsedTime / 60;
		gameTimes_[1] = ((uint16_t)elapsedTime % 60) / 10;
		gameTimes_[2] = ((uint16_t)elapsedTime % 60) % 10;

		if (sBoss_->GetIsAlive() == false) {
			isClear_ = true;
			clearTime_ = gameTimes_;
			ResultScene::SetClearTime(elapsedTime);
		}
	}
}

void UIManager::MatUpdate()
{
	sTitleFrame_->MatUpdate();
	sExp_->MatUpdate(true);
	sExpIn_->MatUpdate(true);
	sExpText_->MatUpdate(true);
	sHpPlayer_->MatUpdate(true);
	sHpPlayerIn_->MatUpdate(true);
	sHpTextPlayer_->MatUpdate(true);
	sLevelTextPlayer_->MatUpdate(true);
	sLevelNumberPlayer_->MatUpdate(true);
	sHpBoss_->MatUpdate(true);
	sHpTextBoss_->MatUpdate(true);
	sNextText_->MatUpdate(true);
	
	for (auto& it : sTimes_) it->MatUpdate();
	sColon_->MatUpdate();
}

void UIManager::Draw()
{
	sTitleFrame_->Draw(hTitleFrame_);
	//sExp_->Draw(hExp_);
	//sExpIn_->Draw(hExpIn_);
	sExpText_->Draw(hExpText_);
	//sHpPlayer_->Draw(hHpPlayer_);
	//sHpPlayerIn_->Draw(hHpPlayerIn_);
	//sHpTextPlayer_->Draw(hHpTextPlayer_);
	sLevelTextPlayer_->Draw(hLevelTextPlayer_);
	sHpBoss_->Draw(hHpBoss_);
	sBoss_->UIDraw();
	sHpTextBoss_->Draw(hHpTextBoss_);
	sNextText_->Draw(hNextText_);
	sTimes_[0]->Draw(hNumbers_[gameTimes_[0]]);
	sTimes_[1]->Draw(hNumbers_[gameTimes_[1]]);
	sTimes_[2]->Draw(hNumbers_[gameTimes_[2]]);
	sColon_->Draw(hColon_);
}

void UIManager::DrawPlayerLevel(int playerLevel)
{
	int n = Util::Clamp(playerLevel, 9, 0);

	sLevelNumberPlayer_->Draw(hNumbers_[n]);
}
