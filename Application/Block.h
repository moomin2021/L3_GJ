#pragma once
#include"Texture.h"
#include"Sprite.h"
#include<memory>
#include"BoxCollider.h"
#include"CollisionManager2D.h"

#include<list>

class Player;
class Piece;

class Block {
#pragma region �����o�ϐ�
private:
	// �C���X�^���X
	static CollisionManager2D* sColMgr2D_;
	static Player* sPlayer_;

	// �X�v���C�g
	std::unique_ptr<Sprite> sprite_ = nullptr;

	// �摜�n���h��
	uint16_t handle_ = 0;

	// �R���C�_�[
	std::unique_ptr<BoxCollider> collider_ = nullptr;

	// �������Ă���s�[�X
	Piece* piece_ = nullptr;

	// �I�t�Z�b�g
	Vector2 offset_ = { 0.0f, 0.0f };

	// �����t���O
	bool isAlive_ = true;

	// �s�[�X�ɏ������Ă��邩�t���O
	bool isPiece_ = true;
#pragma endregion

#pragma region �����o�֐�
public:
	// �R���X�g���N�^
	Block();

	// �f�X�g���N�^
	~Block();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

	// �Փˎ�����
	void OnCollision();

	// �s��X�V����
	void MatUpdate();

	// �u���b�N�̏������v���C���[�ɕς���
	void SetAffChangePlayer();
#pragma endregion

#pragma region �Z�b�^�[�֐�
	// �R���W�����}�l�[�W���[2D��ݒ�
	static void SetColMgr2D(CollisionManager2D* colMgr2D) { sColMgr2D_ = colMgr2D; }

	// �v���C���[��ݒ�
	static void SetPlayer(Player* player) { sPlayer_ = player; }

	// �摜�n���h����ݒ�
	void SetHandle(uint16_t handle) { handle_ = handle; }

	// ���W��ݒ�
	void SetPosition(const Vector2 pos) { sprite_->SetPosition(pos); }

	// �s�[�X��ݒ�
	void SetPiece(Piece* piece) { piece_ = piece; }

	// �I�t�Z�b�g�ݒ�
	void SetOffset(const Vector2& offset) { offset_ = offset; }

	// �����t���O��ݒ�
	void SetIsAlive(bool frag) { isAlive_ = frag; }
#pragma endregion

#pragma region �Q�b�^�[�֐�
	// �I�t�Z�b�g���擾
	const Vector2& GetOffset() { return offset_; }

	// �����t���O���擾
	bool GetIsAlive() { return isAlive_; }
#pragma endregion
};