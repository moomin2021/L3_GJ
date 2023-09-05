#include "Block.h"

//�ÓI�����o�ϐ��̎���
uint16_t Block::cannonTexture =0;
uint16_t Block::blockTexture = 0;
Vector2 Block::blockSize = {0,0};


void Block::StaticInitialize(uint16_t cannonTex, uint16_t blockTex, const Vector2& blockSize)
{
	cannonTexture = cannonTex;
	blockTexture = blockTex;
	Block::blockSize = blockSize;
}

void Block::Initialize(const BlockData& blockData, const ParentData& parent)
{
	this->blockData = blockData;
	sprite.SetSize(blockSize);
	this->parent = parent;
}

void Block::SetParent(const ParentData& parent)
{
	this->parent = parent;
}

void Block::Draw()
{
	//�f�[�^�ɂ���ăe�N�X�`���ς���
	uint16_t tex = blockTexture;
	if (blockData == BlockData::None) {
		tex = cannonTexture;
	}
	sprite.Draw(tex);

}
