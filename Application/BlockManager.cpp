#include "BlockManager.h"
#include "CollisionManager2D.h"

#include <imgui_impl_DX12.h>

BlockManager* BlockManager::GetInstance()
{
	// �C���X�^���X����
	static BlockManager inst;

	// �C���X�^���X��Ԃ�
	return &inst;
}

void BlockManager::Initialize()
{
	// �u���b�N�ɃR���W�����}�l�[�W���[��ݒ�
	Block::SetColMgr2D(CollisionManager2D::GetInstance());
}

void BlockManager::Update()
{
	// �u���b�N�̐����t���O��[OFF]�Ȃ����
	for (auto it = blocks_.begin(); it != blocks_.end();) {
		// �s�[�X�̍X�V
		(*it)->Update();

		// �s�[�X�̐����t���O��[OFF]�ɂȂ�����e���폜
		if ((*it)->GetIsAlive() == false) it = blocks_.erase(it);
		else ++it;
	}

	ImGui::Text("BlockCount = %d", blocks_.size());
}

void BlockManager::Draw()
{
	// �u���b�N
	for (auto& it : blocks_) {
		it->Draw();
	}
}

void BlockManager::OnCollision()
{
	// �u���b�N
	for (auto& it : blocks_) {
		it->OnCollision();
	}
}

void BlockManager::MatUpdate()
{
	// �u���b�N
	for (auto& it : blocks_) {
		it->MatUpdate();
	}
}

Block* BlockManager::CreateBlock(const Vector2& offset, Piece* piece)
{
	// �u���b�N����
	blocks_.emplace_back(std::make_unique<Block>());
	blocks_.back()->Initialize();
	blocks_.back()->SetPiece(piece);
	blocks_.back()->SetOffset(offset);

	// �u���b�N�̃|�C���^��Ԃ�
	return blocks_.back().get();
}

BlockManager::BlockManager() {}

BlockManager::~BlockManager() {}