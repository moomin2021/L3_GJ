#pragma once
#include"Texture.h"
#include"Sprite.h"
#include<memory>
#include"CircleCollider.h"
#include"CollisionManager2D.h"
#include<list>

struct ParentData {
	Vector2* parentPos = nullptr;
	float* parentRot = nullptr;
	Vector2 tileOffset{0,0};
	uint16_t parentTag = 0;
};

enum class BlockData {
	Player,	//�v���C���[
	None,	//�ʏ�u���b�N
	Cannon	//��C

};

class Player;

class Block
{
public://�ÓI�����o�֐�
	
	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="cannonTex">��C�̃e�N�X�`��</param>
	/// <param name="blockTex">�u���b�N�̃e�N�X�`��</param>
	/// <param name="blockSize">�u���b�N�̑傫��</param>
	static void StaticInitialize(uint16_t cannonTex, uint16_t blockTex, const Vector2& blockSize);

	static Vector2 GetBlockSize() { return blockSize; }

	static Block* CreateBlock(const BlockData& blockData, ParentData* parent);

	static void SetPlayer(Player* player);

public://�����o�֐�

	/// <summary>
	/// �u���b�N�̏�����
	/// </summary>
	/// <param name="blockData">�Ȃ�̃u���b�N��</param>
	/// <param name="parent">�e�̃f�[�^(���W&��]+�^�C���̋���)</param>
	void Initialize(const BlockData& blockData, ParentData* parent);

	void SetParent(ParentData* parent) { this->parent = parent; }

	virtual void Draw();

	void Update();

	void OnCollison();

	Vector2 GetOffset()const { return parent->tileOffset; }

	ParentData* GetParent() { return parent; }


private://�ÓI�����o�ϐ�
	static uint16_t cannonTexture;
	static uint16_t blockTexture;
	static Vector2 blockSize;
	//�S�Ẵu���b�N���߲��
	static std::vector<std::unique_ptr<Block>> pAllBlock;
	
	static Player* player;

private://�����o�ϐ�

	//�}�l�[�W��
	CollisionManager2D* colManager = nullptr;
	//�R���C�_�[
	std::unique_ptr<CircleCollider> collider = nullptr;

	std::unique_ptr<Sprite> sprite = nullptr;
	BlockData blockData = BlockData::None;
	ParentData* parent = nullptr;

	uint16_t colliderTag = 0;

private: //�ÓI�����o�֐�

	void ChangeParent(uint16_t baseBlockTag, uint16_t hitBlockTag,uint16_t parentTag,const Vector2& hitOffset);
};

