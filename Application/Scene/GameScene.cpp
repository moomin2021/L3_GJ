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
#pragma region �C���X�^���X�擾
	key_ = Key::GetInstance();				// �L�[�{�[�h����
	blockMgr_ = BlockManager::GetInstance();// �u���b�N�}�l�[�W���[
	blockMgr_->Initialize();
#pragma endregion

	// �J����
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 5.0f, -20.0f });

	// �J�����Z�b�g
	Sprite::SetCamera(camera_.get());
	ParticleEmitter::SetCamera(camera_.get());

	uint16_t blockTex = Texture::GetInstance()->LoadTexture("Resources/piece.png");
	uint16_t cannonTex = Texture::GetInstance()->LoadTexture("Resources/piece_cannon.png");
	uint16_t playerTex = Texture::GetInstance()->LoadTexture("Resources/player.png");

	// �v���C���[
	player = std::make_unique<Player>();
	player->Initialize();

	Block::SetPlayer(player.get());

	// �{�X
	boss_ = std::make_unique<Boss>();
	boss_->Initialize();

	//�w�i
	backGroundTexture= Texture::GetInstance()->LoadTexture("Resources/backGround.png");
	backGroundSprite = std::make_unique<Sprite>();
	backGroundSprite->SetSize({ (float)WinAPI::GetInstance()->GetWidth(),(float)WinAPI::GetInstance()->GetHeight() });
	backGroundSprite->MatUpdate();
}

void GameScene::Update()
{
	// �v���C���[
	player->Update();

	// �{�X
	boss_->Update();

	// �u���b�N�}�l�[�W���[
	blockMgr_->Update();

	// �s�[�X�̐����t���O��[OFF]�Ȃ����
	for (auto it = pieces_.begin(); it != pieces_.end();) {
		// �s�[�X�̍X�V
		(*it)->Update();

		// �s�[�X�̐����t���O��[OFF]�ɂȂ�����e���폜
		if ((*it)->GetIsAlive() == false) it = pieces_.erase(it);
		else ++it;
	}

	if (ImGui::Button("add piece")) {
		CreatePiece();
	}

	// �Փˎ�����
	OnCollision();

	// �I�u�W�F�N�g�X�V����
	MatUpdate();

	ImGui::Text("PieceCount = %d", pieces_.size());
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

	// �{�X
	boss_->Draw();

	// �u���b�N�}�l�[�W���[
	blockMgr_->Draw();
}

void GameScene::MatUpdate()
{
	// �J�����̍X�V
	camera_->Update();

	// �{�X
	boss_->MatUpdate();

	// �v���C���[
	player->MatUpdate();

	// �u���b�N�}�l�[�W���[
	blockMgr_->MatUpdate();
}

void GameScene::OnCollision()
{
	// �S����`�F�b�N
	CollisionManager2D::GetInstance()->CheckAllCollision();

	// �{�X
	boss_->OnCollision();

	// �u���b�N�}�l�[�W���[
	blockMgr_->OnCollision();

	for (auto& it : pieces_) {
		it->OnCollision();
	}

}

void GameScene::CreatePiece()
{
	// �s�[�X����
	pieces_.emplace_back(std::make_unique<Piece>());
	pieces_.back()->Initialize(Vector2{2100.0f, 540.0f});
	pieces_.back()->AddBlock(blockMgr_->CreateBlock(Vector2{ 0.0f, 0.0f }, pieces_.back().get()));
	pieces_.back()->AddBlock(blockMgr_->CreateBlock(Vector2{ -1.0f, 0.0f }, pieces_.back().get()));
	pieces_.back()->AddBlock(blockMgr_->CreateBlock(Vector2{ 0.0f, -1.0f }, pieces_.back().get()));
	pieces_.back()->AddBlock(blockMgr_->CreateBlock(Vector2{ 1.0f, 0.0f }, pieces_.back().get()));
}