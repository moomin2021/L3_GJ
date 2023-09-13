#include "TitleScene.h"
#include "PipelineManager.h"
#include "Texture.h"
#include "Sound.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {
	sound_->Stop(sTitleBGM_);
}

void TitleScene::Initialize()
{
#pragma region �C���X�^���X�擾
	pad_ = Pad::GetInstance();
	key_ = Key::GetInstance();
	sound_ = Sound::GetInstance();
#pragma endregion

#pragma region �J����
	camera_ = std::make_unique<Camera>();
	Sprite::SetCamera(camera_.get());
#pragma endregion

#pragma region �X�v���C�g
	sBackGround_ = std::make_unique<Sprite>();
	sBackGround_->SetSize({ 1856.0f, 960.0f });

	sTitleFrame_ = std::make_unique<Sprite>();
	sTitleFrame_->SetSize({ 1856.0f, 960.0f });

	sTitleLogo_ = std::make_unique<Sprite>();
	sTitleLogo_->SetPosition({ 928.0f, 350.0f });
	sTitleLogo_->SetSize({ 1360.0f, 350.0f });
	sTitleLogo_->SetAnchorPoint({ 0.5f, 0.5f });

	sStartText_ = std::make_unique<Sprite>();
	sStartText_->SetPosition({ 628.0f, 740.0f });
	sStartText_->SetSize({ 198.0f, 62.0f });
	sStartText_->SetAnchorPoint({ 0.5f, 0.5f });

	sExitText_ = std::make_unique<Sprite>();
	sExitText_->SetPosition({ 1228.0f, 740.0f });
	sExitText_->SetSize({ 155.0f, 68.0f });
	sExitText_->SetAnchorPoint({ 0.5f, 0.5f });

	sTitleSelectFrame_ = std::make_unique<Sprite>();
	sTitleSelectFrame_->SetPosition({ 628.0f, 740.0f });
	sTitleSelectFrame_->SetSize({ 257.0f, 107.0f });
	sTitleSelectFrame_->SetAnchorPoint({ 0.5f, 0.5f });
#pragma endregion

#pragma region �摜�n���h��
	hBackGround_ = LoadTexture("Resources/background.png");
	hTitleFrame_ = LoadTexture("Resources/title_frame.png");
	hTitleLogo_ = LoadTexture("Resources/title_logo.png");
	hStartText_ = LoadTexture("Resources/start_text.png");
	hExitText_ = LoadTexture("Resources/exit_text.png");
	hTitleSelectFrame_ = LoadTexture("Resources/title_select_frame.png");
#pragma endregion

#pragma region �T�E���h�n���h��
	sTitleBGM_ = sound_->LoadWave("Resources/Sound/title_bgm.wav", 0.01f);
	selectMoveSE_ = sound_->LoadWave("Resources/Sound/select.wav", 0.01f);
	selectSE_ = sound_->LoadWave("Resources/Sound/decision.wav", 0.01f);
	sound_->Play(sTitleBGM_);
#pragma endregion

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();

}

void TitleScene::Update()
{
	if (key_->TriggerKey(DIK_D)) {
		selectNum_++;
		if (selectNum_ > 1) selectNum_ = 0;
		sound_->Play(selectMoveSE_);
	}

	if (key_->TriggerKey(DIK_A)) {
		selectNum_--;
		if (selectNum_ < 0) selectNum_ = 1;
		sound_->Play(selectMoveSE_);
	}

	if (pad_->GetLStick().x <= -0.6f && oldLStickX > -0.6f) {
		selectNum_++;
		if (selectNum_ > 1) selectNum_ = 0;
		sound_->Play(selectMoveSE_);
	}

	if (pad_->GetLStick().x >= 0.6f && oldLStickX < 0.6f) {
		selectNum_--;
		if (selectNum_ < 0) selectNum_ = 1;
		sound_->Play(selectMoveSE_);
	}

	if (pad_->GetTriggerButton(PAD_A) || key_->TriggerKey(DIK_SPACE)) {
		if (selectNum_ == 0) fade_->ChangeScene(SCENE::GAME);
		if (selectNum_ == 1) SceneManager::GetInstance()->SetIsEnd(true);
		sound_->Play(selectSE_);
	}

	oldLStickX = pad_->GetLStick().x;

	sTitleSelectFrame_->SetPosition(selectPos_[selectNum_]);

	fade_->Update();

	// �Փˎ�����
	OnCollision();

	// �s��X�V����
	MatUpdate();
}

void TitleScene::Draw()
{
	// �X�v���C�g
	PipelineManager::PreDraw("Sprite");

	sBackGround_->Draw(hBackGround_);// �w�i
	sTitleFrame_->Draw(hTitleFrame_);// �t���[��
	sTitleLogo_->Draw(hTitleLogo_);// �^�C�g�����S
	sStartText_->Draw(hStartText_);// �X�^�[�g�e�L�X�g
	sExitText_->Draw(hExitText_);// Exit�e�L�X�g
	sTitleSelectFrame_->Draw(hTitleSelectFrame_);

	fade_->Draw();
}

void TitleScene::OnCollision()
{
	
}

void TitleScene::MatUpdate()
{
	// �J����
	camera_->Update();

	sBackGround_->MatUpdate();// �w�i
	sTitleFrame_->MatUpdate();// �t���[��
	sTitleLogo_->MatUpdate();// �^�C�g�����S
	sStartText_->MatUpdate();
	sExitText_->MatUpdate();
	sTitleSelectFrame_->MatUpdate();
	fade_->MatUpdate();
}