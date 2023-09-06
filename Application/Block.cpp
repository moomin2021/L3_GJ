#include "Block.h"
#include"Util.h"

//静的メンバ変数の実態
uint16_t Block::cannonTexture = 0;
uint16_t Block::blockTexture = 0;
Vector2 Block::blockSize = { 0,0 };


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
	Vector2 vecB;

	//親からのオフセットで角度を作成
	Vector2 forward = { 1,0 };
	Vector2 offset = parent->tileOffset;
	offset.normalize();

	float theta = (forward.x * offset.x) + (forward.y * offset.y) / (forward.length()) * (offset.length());
	theta = acosf(theta);

	//度数法に変換
	theta = Util::Radian2Degree(theta);
	//オフセットのYがマイナスなら360から引く
	if (offset.y > 0) {
		theta = 360.0f - theta;
	}

	vecB.x =blockSize.x * parent->tileOffset.x;
	vecB.y =blockSize.y * parent->tileOffset.y;

	float lenPtoB = vecB.length();
	pos.x += lenPtoB * cosf(Util::Degree2Radian(theta + *parent->parentRot));
	pos.y += lenPtoB * sinf(Util::Degree2Radian(theta+ *parent->parentRot));

	sprite->SetPosition(pos);
	//親の回転をブロックの回転に適用
	sprite->SetRotation(*parent->parentRot);
	sprite->MatUpdate();

}
