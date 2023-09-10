#include "UIManager.h"
#include "Texture.h"

UIManager::UIManager() {}

UIManager::~UIManager() {}

void UIManager::Initialize()
{
#pragma region スプライト
	sFrame_ = std::make_unique<Sprite>();
	sFrame_->SetSize({ 1920.0f, 1080.0f });

	sPlayerHpIn_ = std::make_unique<Sprite>();
	sPlayerHpIn_->SetPosition({ 300.0f, 1030.0f });
	sPlayerHpIn_->SetSize({ 504.0f, 40.0f });
	sPlayerHpIn_->SetAnchorPoint({ 0.5f, 0.5f });
#pragma endregion

#pragma region 画像ハンドル
	hFrame_ = LoadTexture("Resources/frame.png");
	hPlayerHpIn_ = LoadTexture("Resources/hp_player_in.png");
#pragma endregion
}

void UIManager::MatUpdate()
{
	sFrame_->MatUpdate();
	sPlayerHpIn_->MatUpdate();
}

void UIManager::Draw()
{
	sFrame_->Draw(hFrame_);
	sPlayerHpIn_->Draw(hPlayerHpIn_);
}