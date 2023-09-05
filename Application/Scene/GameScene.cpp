#include "GameScene.h"
#include "PipelineManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize()
{
#pragma region �C���X�^���X
	key_ = Key::GetInstance();// �L�[�{�[�h����
#pragma endregion

#pragma region �J����
	camera_ = std::make_unique<Camera>();	// ����
	Sprite::SetCamera(camera_.get());		// �X�v���C�g�ɃJ������ݒ�
#pragma endregion
}

void GameScene::Update()
{
	// �Փˎ�����
	OnCollision();

	// �I�u�W�F�N�g����
	ObjUpdate();
}

void GameScene::Draw()
{
	// �X�v���C�g�O����
	PipelineManager::GetInstance()->PreDraw("Sprite");
}

void GameScene::OnCollision()
{

}

void GameScene::ObjUpdate()
{
	// �J�����X�V
	camera_->Update();
}