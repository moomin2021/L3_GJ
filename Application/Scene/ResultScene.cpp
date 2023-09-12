#include "ResultScene.h"
#include "PipelineManager.h"
#include "Texture.h"

ResultScene::ResultScene() {}

ResultScene::~ResultScene() {}

void ResultScene::Initialize()
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
	sBackGround_->SetSize({ 1856.0f, 960.0f });
#pragma endregion

#pragma region �摜�n���h��
	hBackGround_ = LoadTexture("Resources/background.png");
#pragma endregion
}

void ResultScene::Update()
{
	// �Փˎ�����
	OnCollision();

	// �s��X�V����
	MatUpdate();
}

void ResultScene::Draw()
{
	// �X�v���C�g
	PipelineManager::PreDraw("Sprite");

	// �w�i
	sBackGround_->Draw(hBackGround_);
}

void ResultScene::OnCollision()
{

}

void ResultScene::MatUpdate()
{
	// �J����
	camera_->Update();

	// �w�i
	sBackGround_->MatUpdate();
}