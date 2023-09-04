#include "TemplateScene.h"

TemplateScene::TemplateScene() :
	key_(nullptr),
	camera_(nullptr)
{
}

TemplateScene::~TemplateScene()
{
	delete camera_;
}

void TemplateScene::Initialize()
{
	// �L�[�{�[�h���̓C���X�^���X�擾
	key_ = Key::GetInstance();

	// �J����
	camera_ = new Camera();
	//camera_->eye_ = { 0.0f, 10.0f, -30.0f };
}

void TemplateScene::Update()
{
	// �J�����̍X�V
	camera_->Update();
}

void TemplateScene::Draw()
{
}