#include "Block.h"

//静的メンバ変数の実態
uint16_t Block::cannonTexture =0;
uint16_t Block::blockTexture = 0;
Vector2 Block::blockSize = {0,0};


void Block::StaticInitialize(uint16_t cannonTex, uint16_t blockTex, const Vector2& blockSize)
{
	cannonTexture = cannonTex;
	blockTexture = blockTex;
	Block::blockSize = blockSize;
}

void Block::Initialize(const BlockData& blockData, ParentData* parent)
{
	this->blockData = blockData;
	sprite = std::make_unique<Sprite>();
	sprite->SetSize(blockSize);
	sprite->SetAnchorPoint({ 0.5f,0.5f });
	this->parent = parent;
}



void Block::Draw()
{
	//データによってテクスチャ変える
	uint16_t tex = blockTexture;
	if (blockData == BlockData::None) {
		tex = cannonTexture;
	}
	sprite->Draw(tex);

}

void Block::Update()
{
	//親の座標に依存して座標移動
	if (!parent) {
		return;
	}

	Vector2 pos = *parent->parentPos;
	pos.x += blockSize.x * parent->tileOffset.x;
	pos.y += blockSize.y * parent->tileOffset.y;

	sprite->SetPosition(pos);
	sprite->MatUpdate();
}
