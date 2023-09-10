#include "TitleScene.h"
#include "PipelineManager.h"
#include "Texture.h"
#include "SceneManager.h"

TitleScene::TitleScene() {}

TitleScene::~TitleScene() {}

void TitleScene::Initialize()
{
#pragma region �C���X�^���X�擾
	pad_ = Pad::GetInstance();
#pragma endregion

#pragma region �J����
	camera_ = std::make_unique<Camera>();
	Sprite::SetCamera(camera_.get());
#pragma endregion

#pragma region �X�v���C�g
	sBackGround_ = std::make_unique<Sprite>();
	sBackGround_->SetSize({ 1920.0f, 1080.0f });

	sTitleFrame_ = std::make_unique<Sprite>();
	sTitleFrame_->SetSize({ 1920.0f, 1080.0f });

	sTitleLogo_ = std::make_unique<Sprite>();
	sTitleLogo_->SetPosition({ 960.0f, 385.0f });
	sTitleLogo_->SetSize({ 1360.0f, 350.0f });
	sTitleLogo_->SetAnchorPoint({ 0.5f, 0.5f });

	sPressA_ = std::make_unique<Sprite>();
	sPressA_->SetPosition({ 960.0f, 830.0f });
	sPressA_->SetSize({314.0f, 64.0f});
	sPressA_->SetAnchorPoint({ 0.5f, 0.5f });
#pragma endregion

#pragma region �摜�n���h��
	hBackGround_ = LoadTexture("Resources/background.png");
	hTitleFrame_ = LoadTexture("Resources/title_frame.png");
	hTitleLogo_ = LoadTexture("Resources/title_logo.png");
	hPressA_ = LoadTexture("Resources/pressA.png");
#pragma endregion
}

void TitleScene::Update()
{
	// �Փˎ�����
	OnCollision();

	// �s��X�V����
	MatUpdate();

	// A�{�^������������Q�[���V�[���ɐ؂�ւ���
	if (pad_->GetTriggerButton(PAD_A)) {
		SceneManager::GetInstance()->ChangeScene(SCENE::GAME);
	}
}

void TitleScene::Draw()
{
	// �X�v���C�g
	PipelineManager::PreDraw("Sprite");

	sBackGround_->Draw(hBackGround_);// �w�i
	sTitleFrame_->Draw(hTitleFrame_);// �t���[��
	sTitleLogo_->Draw(hTitleLogo_);// �^�C�g�����S
	sPressA_->Draw(hPressA_);// �v���XA
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
	sPressA_->MatUpdate();// �v���XA
}