#include "Scene1.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"
#include "Sound.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "ImGuiManager.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_DX12.h>

Scene1::Scene1() :
	key_(nullptr)
{
}

Scene1::~Scene1() {}

void Scene1::Initialize()
{
	// �L�[�{�[�h���̓C���X�^���X�擾
	key_ = Key::GetInstance();

	// �J����
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 5.0f, -20.0f });

	// �J�����Z�b�g
	Object3D::SetCamera(camera_.get());
	ParticleEmitter::SetCamera(camera_.get());

	// ���C�g�O���[�v����
	lightGroup_ = std::make_unique<LightGroup>();

	// ���C�g�O���[�v�Z�b�g
	Object3D::SetLightGroup(lightGroup_.get());

	// ���s��������
	dirLight_ = std::make_unique<DirectionalLight>();

	// ���s�����Z�b�g
	lightGroup_->AddDirLight(dirLight_.get());

	// �T�E���h�ǂݍ��݁��Đ�
	bgmKey_ = Sound::LoadWave("Resources/Sound/a.wav");
	Sound::SetVolume(bgmKey_, 0.001f);
	Sound::Play(bgmKey_);

	// �v���C���[
	player_ = std::make_unique<TestPlayer>();
	player_->Initialize();

	int a = 0;
	a++;

	// �G�l�~�[
	enemy_ = std::make_unique<TestEnemy>();
	enemy_->Initialize();
}

void Scene1::Update()
{
	// �v���C���[�X�V
	player_->Update();

	// �G�l�~�[�X�V
	enemy_->Update();

	// �Փˎ�����
	OnCollision();

	// �I�u�W�F�N�g�X�V����
	ObjUpdate();

	// �J�����̍X�V
	camera_->Update();

	// �Փ˔���
	CollisionManager::GetInstance()->CheckAllCollision();
}

void Scene1::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// �v���C���[�`��
	player_->Draw();

	// �G�l�~�[�`��
	enemy_->Draw();
}

void Scene1::ObjUpdate()
{
	player_->ObjUpdate();
	enemy_->ObjUpdate();
}

void Scene1::OnCollision()
{
	player_->OnCollision();
	enemy_->OnCollision();
}