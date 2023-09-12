#include "GameScene.h"
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

GameScene::GameScene() :
	key_(nullptr)
{
}

GameScene::~GameScene() {}

void GameScene::Initialize()
{
	// �L�[�{�[�h���̓C���X�^���X�擾
	key_ = Key::GetInstance();

	// �J����
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 5.0f, -20.0f });

	// �J�����Z�b�g
	Sprite::SetCamera(camera_.get());
	ParticleEmitter2D::SetCamera(camera_.get());

	uint16_t blockTex = Texture::GetInstance()->LoadTexture("Resources/piece.png");
	uint16_t cannonTex = Texture::GetInstance()->LoadTexture("Resources/piece_cannon.png");
	uint16_t playerTex = Texture::GetInstance()->LoadTexture("Resources/player.png");
	particleHandle_ = LoadTexture("Resources/effect2.png");


	//�u���b�N�N���X�ÓI������
	Block::StaticInitialize(blockTex, cannonTex,playerTex, { 32,32 });

	// �v���C���[
	Vector2 playerPos = { Block::GetBlockSize().x * 3.0f + (Block::GetBlockSize().x / 2.0f),
	Block::GetBlockSize().y * 15.0f - (Block::GetBlockSize().y / 2.0f) };


	player = std::make_unique<Player>();
	player->Initialize(playerTex,playerPos);
	PlayerBullet::SetColMgr2D(CollisionManager2D::GetInstance());

	//���@�̒e�̃e�N�X�`���̓ǂݍ��݂ƃZ�b�g
	PlayerBullet::SetHandle(LoadTexture("Resources/bullet_player.png"));

	//�v���C���[�ƃs�[�X�z��̃Z�b�g
	Block::SetPlayer(player.get());
	Block::SetPiece(&Piece::pieces);


	// �{�X
	boss_ = std::make_unique<Boss>();
	boss_->Initialize();
	PlayerBullet::SetBoss(boss_.get());

	//�w�i
	backGroundTexture= Texture::GetInstance()->LoadTexture("Resources/backGround.png");
	backGroundSprite = std::make_unique<Sprite>();
	backGroundSprite->SetSize({ (float)WinAPI::GetInstance()->GetWidth(),(float)WinAPI::GetInstance()->GetHeight() });
	backGroundSprite->MatUpdate();

	// UI�}�l�[�W���[
	uiMgr_ = std::make_unique<UIManager>();
	uiMgr_->Initialize();
}

void GameScene::Update()
{

	// �v���C���[
	player->Update();

	// �{�X
	boss_->Update();

	Piece::ALlPieceUpdate();
	ImGui::Text("all block %d,", (int)Block::GetBlockCount());

	Block::AllBlockDeleteCheck();

	// �Փˎ�����
	OnCollision();

	// �I�u�W�F�N�g�X�V����
	MatUpdate();
}

void GameScene::Draw()
{
	// �w�i�X�v���C�g
	PipelineManager::PreDraw("Sprite");

	// �w�i
	backGroundSprite->Draw(backGroundTexture);

	// �O�i�X�v���C�g
	PipelineManager::PreDraw("Sprite");

	// �v���C���[
	player->Draw();

	for (size_t i = 0; i < Piece::pieces.size(); i++) {
		Piece::pieces[i]->Draw();
	}

	// �{�X
	boss_->Draw();

	PipelineManager::PreDraw("Sprite");

	// UI�}�l�[�W���[
	uiMgr_->Draw();
}

void GameScene::MatUpdate()
{
	// �J�����̍X�V
	camera_->Update();

	//�v���C���[
	player->MatUpdate();

	// �{�X
	boss_->MatUpdate();

	// UI�}�l�[�W���[
	uiMgr_->MatUpdate();
}

void GameScene::OnCollision()
{
	// �S����`�F�b�N
	CollisionManager2D::GetInstance()->CheckAllCollision();

	// �{�X
	boss_->OnCollision();

	player->OnCollision();

	//����
	for (size_t i = 0; i < Piece::pieces.size(); i++) {
		Piece::pieces[i]->OnCollision();
	}

}