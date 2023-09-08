#include "Scene1.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"
#include "Sound.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "ImGuiManager.h"
#include"WinAPI.h"

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
	Sprite::SetCamera(camera_.get());
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

	uint16_t blockTex = Texture::GetInstance()->LoadTexture("Resources/piece.png");
	uint16_t cannonTex = Texture::GetInstance()->LoadTexture("Resources/piece_cannon.png");
	uint16_t playerTex = Texture::GetInstance()->LoadTexture("Resources/player.png");


	//�u���b�N�N���X�ÓI������
	Block::StaticInitialize(blockTex, cannonTex, { 32,32 });

	// �v���C���[
	player_ = std::make_unique<TestPlayer>();
	player_->Initialize();

	player = std::make_unique<Player>();
	player->Initialize(playerTex,{96,(float)WinAPI::GetInstance()->GetHeight()/2});

	// �G�l�~�[
	enemy_ = std::make_unique<TestEnemy>();
	enemy_->Initialize();

	//�w�i
	backGroundTexture= Texture::GetInstance()->LoadTexture("Resources/backGround.png");
	backGroundSprite = std::make_unique<Sprite>();
	backGroundSprite->SetSize({ (float)WinAPI::GetInstance()->GetWidth(),(float)WinAPI::GetInstance()->GetHeight() });
	backGroundSprite->MatUpdate();
}

void Scene1::Update()
{
	// �v���C���[�X�V
	player_->Update();

	player->Update();

	//�s�[�X�̍X�V�ƃ{�^���Ő���
	for (size_t i = 0; i < Piece::pieces.size(); i++) {
		Piece::pieces[i]->Update();
	}

	ImGui::Text("piece size %d", Piece::pieces.size());

	//if (ImGui::Button("add piece")) {
	//	std::unique_ptr<Piece> newPiece = std::make_unique<Piece>();
	//	newPiece->Initialize();
	//	pieces.push_back(std::move(newPiece));
	//}

	Piece::CreatePiece();

	// �G�l�~�[�X�V
	enemy_->Update();

	// �Փˎ�����
	OnCollision();

	// �I�u�W�F�N�g�X�V����
	ObjUpdate();

	// �J�����̍X�V
	camera_->Update();
}

void Scene1::Draw()
{
	
	PipelineManager::PreDraw("Object3D");

	// �v���C���[�`��
	player_->Draw();

	// �G�l�~�[�`��
	enemy_->Draw();

	PipelineManager::PreDraw("Sprite");

	player->Draw();

	for (size_t i = 0; i < Piece::pieces.size(); i++) {
		Piece::pieces[i]->Draw();
	}

	backGroundSprite->Draw(backGroundTexture);

}

void Scene1::ObjUpdate()
{
	player_->ObjUpdate();
	enemy_->ObjUpdate();
}

void Scene1::OnCollision()
{
	CollisionManager2D::GetInstance()->CheckAllCollision();

	player_->OnCollision();
	enemy_->OnCollision();

	//����
	for (size_t i = 0; i < Piece::pieces.size(); i++) {
		Piece::pieces[i]->OnCollision();
	}

}