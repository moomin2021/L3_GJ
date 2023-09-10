#pragma once
#include "Block.h"
#include "Piece.h"

#include <memory>
#include <vector>

class BlockManager
{
#pragma region �����o�ϐ�
private:
	// �u���b�N
	std::vector<std::unique_ptr<Block>> blocks_ = {};
#pragma endregion

#pragma region �����o�֐�
public:
	// �C���X�^���X�擾
	static BlockManager* GetInstance();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �Փˎ�����
	void OnCollision();

	// �s��X�V
	void MatUpdate();

	// �u���b�N���쐬
	Block* CreateBlock(const Vector2& offset, Piece* piece);

private:
	// �R���X�g���N�^
	BlockManager();

	// �f�X�g���N�^
	~BlockManager();
#pragma endregion

#pragma region ����֐�
	BlockManager(const BlockManager&) = delete;				// �R�s�[�R���X�g���N�^�֎~
	BlockManager& operator = (const BlockManager&) = delete;	// �R�s�[������Z�q�֎~
#pragma endregion
};