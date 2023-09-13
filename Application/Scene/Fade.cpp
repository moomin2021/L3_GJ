#include "Fade.h"
#include "Texture.h"
#include "Util.h"

bool Fade::isFade_ = false;
bool Fade::isFadeIn_ = false;
bool Fade::isFadeOut_ = false;
SCENE Fade::nextScene_ = SCENE::TITLE;

void Fade::Initialize()
{
	// スプライト
	sprite_ = std::make_unique<Sprite>();
	sprite_->SetSize({ 1856.0f, 960.0f });

	// 画像ハンドル
	handle_ = LoadTexture("Resources/fade.png");

	startTime_ = Util::GetTimrMSec();
}

void Fade::Update()
{
	if (isFade_) {
		float elapsed = (Util::GetTimrMSec() - startTime_) / 1000.0f;
		
		if (isFadeIn_) {
			float rate = elapsed / fadeInTime_;
			rate = Util::Clamp(rate, 1.0f, 0.0f);

			sprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f - rate });

			if (elapsed > fadeInTime_) {
				isFade_ = false;
				isFadeIn_ = false;
			}
		}

		if (isFadeOut_) {
			float rate = elapsed / fadeOutTime_;
			rate = Util::Clamp(rate, 1.0f, 0.0f);

			sprite_->SetColor({ 1.0f, 1.0f, 1.0f, rate });

			if (elapsed > fadeOutTime_) {
				isFade_ = true;
				isFadeIn_ = true;
				isFadeOut_ = false;
				sprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
				SceneManager::GetInstance()->ChangeScene(nextScene_);
			}
		}
	}
}

void Fade::Draw()
{
	if (isFade_) {
		sprite_->Draw(handle_);
	}
}

void Fade::MatUpdate()
{
	sprite_->MatUpdate();
}

void Fade::ChangeScene(SCENE sceneNum)
{
	if (isFade_) return;

	isFade_ = true;
	isFadeOut_ = true;
	nextScene_ = sceneNum;
	startTime_ = Util::GetTimrMSec();
	sprite_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f });
}