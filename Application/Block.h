#pragma once
#include"Texture.h"
#include"Sprite.h"
#include<memory>

struct ParentData {
	Vector2* parentPos = nullptr;
	Vector2 tileOffset{0,0};
};

enum class BlockData {
	Player,	//�v���C���[
	None,	//�ʏ�u���b�N
	Cannon	//��C

};

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

public://�����o�֐�

	/// <summary>
	/// �u���b�N�̏�����
	/// </summary>
	/// <param name="blockData">�Ȃ�̃u���b�N��</param>
	/// <param name="parent">�e�̃f�[�^(���W+�^�C���̋���)</param>
	void Initialize(const BlockData& blockData, ParentData* parent);

	void SetParent(ParentData* parent) { this->parent = parent; }

	virtual void Draw();

	void Update();


private://�ÓI�����o�ϐ�
	static uint16_t cannonTexture;
	static uint16_t blockTexture;
	static Vector2 blockSize;

private://�����o�ϐ�

	//TODO:�R���C�_�[�̒�`

	std::unique_ptr<Sprite> sprite = nullptr;
	BlockData blockData = BlockData::None;
	ParentData* parent = nullptr;



};

