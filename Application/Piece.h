#pragma once
#include"Block.h"
#include<vector>

class Piece {
#pragma region �����o�ϐ�
private:
	// �u���b�N
	std::vector <Block*> blocks_;

	// ���W
	Vector2 position_ = { 0.0f, 0.0f };

	// �ړ����x
	float moveSpd_ = 16.0f;

	// �����t���O
	bool isAlive_ = true;

	// �Փ˃t���O
	bool isCol_ = false;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	Piece();

	// �f�X�g���N�^
	~Piece();

	// ����������
	void Initialize(const Vector2& pos);

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �Փˎ�����
	void OnCollision();

	// �s��X�V����
	void MatUpdate();

	// �u���b�N�ǉ�
	void AddBlock(Block* block) { blocks_.emplace_back(block); }
#pragma endregion

#pragma region �Z�b�^�[�֐�
	// �����t���O��ݒ�
	void SetIsAlive(bool frag) { isAlive_ = frag; }

	// �Փ˃t���O��ݒ�
	void SetIsCol(bool frag) { isCol_ = frag; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	// �����t���O���擾
	bool GetIsAlive() { return isAlive_; }

	// �Փ˃t���O���擾
	bool GetIsCol() { return isCol_; }
#pragma endregion
};