#include "UIManager.h"
#include "Texture.h"

UIManager::UIManager() {}

UIManager::~UIManager() {}

void UIManager::Initialize()
{
#pragma region スプライト
	sFrame_ = std::make_unique<Sprite>();
	sFrame_->SetSize({ 1920.0f, 1080.0f });

	sExp_ = std::make_unique<Sprite>();
	sExp_->SetPosition({ 344.0f, 1035.0f });
	sExp_->SetSize({ 640.0f, 48.0f });
	sExp_->SetAnchorPoint({ 0.5f, 0.5f });

	sExpIn_ = std::make_unique<Sprite>();
	sExpIn_->SetPosition({ 28.0f, 1015.0f });
	sExpIn_->SetSize({ 632.0f, 40.0f });

	sExpText_ = std::make_unique<Sprite>();
	sExpText_->SetPosition({ 60.0f, 1015.0f });
	sExpText_->SetSize({ 120.0f, 48.0f });
	sExpText_->SetAnchorPoint({ 0.5f, 0.5f });
	sExpText_->SetRotation(-15.0f);

	sHpPlayer_ = std::make_unique<Sprite>();
	sHpPlayer_->SetPosition({ 280.0f, 45.0f });
	sHpPlayer_->SetSize({ 512.0f, 48.0f });
	sHpPlayer_->SetAnchorPoint({ 0.5f, 0.5f });

	sHpPlayerIn_ = std::make_unique<Sprite>();
	sHpPlayerIn_->SetPosition({ 28.0f, 25.0f });
	sHpPlayerIn_->SetSize({ 504.0f, 40.0f });

	sHpTextPlayer_ = std::make_unique<Sprite>();
	sHpTextPlayer_->SetPosition({ 50.0f, 25.0f });
	sHpTextPlayer_->SetSize({ 98.0f, 48.0f });
	sHpTextPlayer_->SetAnchorPoint({ 0.5f, 0.5f });
	sHpTextPlayer_->SetRotation(-15.0f);

	sHpBoss_ = std::make_unique<Sprite>();
	sHpBoss_->SetPosition({ 1500.0f, 1035.0f });
	sHpBoss_->SetSize({ 800.0f, 48.0f });
	sHpBoss_->SetAnchorPoint({ 0.5f, 0.5f });

	sHpBossIn_ = std::make_unique<Sprite>();
	sHpBossIn_->SetPosition({ 1104.0f, 1015.0f });
	sHpBossIn_->SetSize({ 792.0f, 40.0f });

	sHpTextBoss_ = std::make_unique<Sprite>();
	sHpTextBoss_->SetPosition({ 1126.0f, 1015.0f });
	sHpTextBoss_->SetSize({ 98.0f, 48.0f });
	sHpTextBoss_->SetAnchorPoint({ 0.5f, 0.5f });
	sHpTextBoss_->SetRotation(-15.0f);

	sKakeru2_ = std::make_unique<Sprite>();
	sKakeru2_->SetPosition({ 1890.0f, 1058.0f });
	sKakeru2_->SetSize({ 64.0f, 29.0f });
	sKakeru2_->SetAnchorPoint({ 0.5f, 0.5f });
	sKakeru2_->SetRotation(-15.0f);

	sNextText_ = std::make_unique<Sprite>();
	sNextText_->SetPosition({ 1870.0f, 90.0f });
	sNextText_->SetSize({ 100.0f, 32.0f });
	sNextText_->SetAnchorPoint({ 0.5f, 0.5f });
	sNextText_->SetRotation(-15.0f);
#pragma endregion

#pragma region 画像ハンドル
	hFrame_ = LoadTexture("Resources/frame.png");
	hExpIn_ = LoadTexture("Resources/hp_player_in.png");
	hExp_ = LoadTexture("Resources/hp_player.png");
	hExpText_ = LoadTexture("Resources/exp_text.png");
	hHpPlayer_ = LoadTexture("Resources/hp_player.png");
	hHpPlayerIn_ = LoadTexture("Resources/hp_player_in.png");
	hHpTextPlayer_ = LoadTexture("Resources/hp_text_player.png");
	hHpBoss_ = LoadTexture("Resources/hp_boss.png");
	hHpBossIn_ = LoadTexture("Resources/hp_boss_in.png");
	hHpTextBoss_ = LoadTexture("Resources/hp_text_boss.png");
	hKakeru2_ = LoadTexture("Resources/kakeru2.png");
	hNextText_ = LoadTexture("Resources/next_text.png");
#pragma endregion
}

void UIManager::MatUpdate()
{
	sFrame_->MatUpdate();
	sExp_->MatUpdate();
	sExpIn_->MatUpdate();
	sExpText_->MatUpdate();
	sHpPlayer_->MatUpdate();
	sHpPlayerIn_->MatUpdate();
	sHpTextPlayer_->MatUpdate();
	sHpBoss_->MatUpdate();
	sHpBossIn_->MatUpdate();
	sHpTextBoss_->MatUpdate();
	sKakeru2_->MatUpdate();
	sNextText_->MatUpdate();
}

void UIManager::Draw()
{
	sFrame_->Draw(hFrame_);
	sExp_->Draw(hExp_);
	sExpIn_->Draw(hExpIn_);
	sExpText_->Draw(hExpText_);
	sHpPlayer_->Draw(hHpPlayer_);
	sHpPlayerIn_->Draw(hHpPlayerIn_);
	sHpTextPlayer_->Draw(hHpTextPlayer_);
	sHpBoss_->Draw(hHpBoss_);
	sHpBossIn_->Draw(hHpBossIn_);
	sHpTextBoss_->Draw(hHpTextBoss_);
	sKakeru2_->Draw(hKakeru2_);
	sNextText_->Draw(hNextText_);
}