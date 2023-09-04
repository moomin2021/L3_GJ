#include "Scene2.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"
#include "Sound.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "ImGuiManager.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_DX12.h>

Scene2::Scene2() :
	key_(nullptr)
{
}

Scene2::~Scene2()
{
	CollisionManager::GetInstance()->RemoveCollider(rayCol_.get());
	CollisionManager::GetInstance()->RemoveCollider(meshCol_.get());
}

void Scene2::Initialize()
{
	// �L�[�{�[�h���̓C���X�^���X�擾
	key_ = Key::GetInstance();

	// �J����
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 1.0f, -20.0f });

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

	// ���f������
	model_ = std::make_unique<Model>("floor");

	// �I�u�W�F�N�g����
	object_ = std::make_unique<Object3D>(model_.get());
	object_->SetPosition({ -1.0f, 0.0f, 0.0f });

	particleEmitters_.resize(10);
	// �p�[�e�B�N���G�~�b�^�[����
	for (size_t i = 0; i < 10; i++) {
		particleEmitters_[i] = std::make_unique<ParticleEmitter>();
		particleEmitters_[i]->SetSpawnPos({ (i * 1.0f) - 4.5f, 0.0f, 0.0f });

		for (size_t j = 0; j < 1000; j++) {
			// �p�[�e�B�N������
			Vector3 pos{};// ���W
			pos.x = Util::GetRandomFloat(-0.1f, 0.1f);
			pos.y = Util::GetRandomFloat(-0.1f, 0.1f);
			pos.z = Util::GetRandomFloat(-0.1f, 0.1f);

			// ����
			Vector3 vel{};
			vel.x = Util::GetRandomFloat(-0.1f, 0.1f);
			vel.y = Util::GetRandomFloat(-0.1f, 0.1f);
			vel.z = Util::GetRandomFloat(-0.1f, 0.1f);

			// �����x
			Vector3 acc{};
			acc.x = Util::GetRandomFloat(-0.001f, 0.0f);
			acc.y = Util::GetRandomFloat(-0.001f, 0.0f);
			acc.z = Util::GetRandomFloat(-0.001f, 0.0f);

			particleEmitters_[i]->Add(1000, pos, vel, acc, 0.5f, 0.0f);
		}
	}

	// �p�[�e�B�N���p�摜�ǂݍ���
	particlehandle_ = LoadTexture("Resources/effect1.png");

	// �T�E���h�ǂݍ��݁��Đ�
	bgmKey_ = Sound::LoadWave("Resources/Sound/a.wav");
	Sound::SetVolume(bgmKey_, 0.001f);
	Sound::Play(bgmKey_);

#pragma region �R���C�_�[
	rayCol_ = std::make_unique<RayCollider>(Vector3{ 0.0f, 2.0f, 0.0f }, Vector3{ 0.0f, -1.0f, 0.0f });
	rayCol_->SetAttribute(COL_ATTR_ALL);
	meshCol_ = std::make_unique<MeshCollider>(object_.get());
	meshCol_->SetAttribute(COL_ATTR_ALL);

	CollisionManager::GetInstance()->AddCollider(rayCol_.get());
	CollisionManager::GetInstance()->AddCollider(meshCol_.get());
#pragma endregion
}

void Scene2::Update()
{
	// �p�[�e�B�N���X�V
	for (auto& i : particleEmitters_) {
		i->Update(BILLBOARD::ALL);
	}

	Vector3 inter = {};

	if (meshCol_->GetIsHit()) {
		inter = meshCol_->GetInter();
	}

	ImGui::Begin("Test");

	ImGui::Text("%f, %f, %f", inter.x, inter.y, inter.z);

	ImGui::End();

	// �I�u�W�F�N�g�X�V
	object_->Update();

	// �J�����̍X�V
	camera_->Update();

	// �Փ˔���
	CollisionManager::GetInstance()->CheckAllCollision();
}

void Scene2::Draw()
{
	PipelineManager::PreDraw("Object3D");

	// �I�u�W�F�N�g�`��
	object_->Draw();

	PipelineManager::PreDraw("Particle", D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	// �p�[�e�B�N���`��
	for (auto& i : particleEmitters_) {
		i->Draw(particlehandle_);
	}
}