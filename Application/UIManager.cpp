#include "UIManager.h"
#include "Texture.h"
#include "Boss.h"

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
#pragma endregion

#pragma region 画像ハンドル
	hTitleFrame_ = LoadTexture("Resources/frame.png");
	hExpIn_ = LoadTexture("Resources/hp_player_in.png");
	hExp_ = LoadTexture("Resources/hp_player.png");
	hExpText_ = LoadTexture("Resources/exp_text.png");
	hHpPlayer_ = LoadTexture("Resources/hp_player.png");
	hHpPlayerIn_ = LoadTexture("Resources/hp_player_in.png");
	hHpTextPlayer_ = LoadTexture("Resources/hp_text_player.png");
	hHpBoss_ = LoadTexture("Resources/hp_boss.png");
	hHpTextBoss_ = LoadTexture("Resources/hp_text_boss.png");
	hNextText_ = LoadTexture("Resources/next_text.png");
#pragma endregion
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
	sHpBoss_->MatUpdate(true);
	sHpTextBoss_->MatUpdate(true);
	sNextText_->MatUpdate(true);
}

void UIManager::Draw()
{
	sTitleFrame_->Draw(hTitleFrame_);
	sExp_->Draw(hExp_);
	sExpIn_->Draw(hExpIn_);
	sExpText_->Draw(hExpText_);
	sHpPlayer_->Draw(hHpPlayer_);
	sHpPlayerIn_->Draw(hHpPlayerIn_);
	sHpTextPlayer_->Draw(hHpTextPlayer_);
	sHpBoss_->Draw(hHpBoss_);
	sBoss_->UIDraw();
	sHpTextBoss_->Draw(hHpTextBoss_);
	sNextText_->Draw(hNextText_);
}