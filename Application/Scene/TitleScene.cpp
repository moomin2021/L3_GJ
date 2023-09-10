#include "TitleScene.h"
#include "PipelineManager.h"
#include "Texture.h"

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
#pragma endregion

#pragma region �摜�n���h��
	hBackGround_ = LoadTexture("Resources/background.png");
#pragma endregion
}

void TitleScene::Update()
{
	// �Փˎ�����
	OnCollision();

	// �s��X�V����
	MatUpdate();
}

void TitleScene::Draw()
{
	// �X�v���C�g
	PipelineManager::PreDraw("Sprite");

	// �w�i
	sBackGround_->Draw(hBackGround_);
}

void TitleScene::OnCollision()
{

}

void TitleScene::MatUpdate()
{
	// �J����
	camera_->Update();

	// �w�i
	sBackGround_->MatUpdate();
}