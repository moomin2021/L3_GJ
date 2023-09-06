#include "Block.h"
#include"Util.h"

//�ÓI�����o�ϐ��̎���
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
	//�f�[�^�ɂ���ăe�N�X�`���ς���
	uint16_t tex = blockTexture;
	if (blockData == BlockData::None) {
		tex = cannonTexture;
	}
	sprite->Draw(tex);

}

void Block::Update()
{
	//�e�̍��W�Ɉˑ����č��W�ړ�
	if (!parent) {
		return;
	}

	Vector2 pos = *parent->parentPos;
	Vector2 vecB;

	//�e����̃I�t�Z�b�g�Ŋp�x���쐬
	Vector2 forward = { 1,0 };
	Vector2 offset = parent->tileOffset;
	offset.normalize();

	float theta = (forward.x * offset.x) + (forward.y * offset.y) / (forward.length()) * (offset.length());
	theta = acosf(theta);

	//�x���@�ɕϊ�
	theta = Util::Radian2Degree(theta);
	//�I�t�Z�b�g��Y���}�C�i�X�Ȃ�360�������
	if (offset.y > 0) {
		theta = 360.0f - theta;
	}

	vecB.x =blockSize.x * parent->tileOffset.x;
	vecB.y =blockSize.y * parent->tileOffset.y;

	float lenPtoB = vecB.length();
	pos.x += lenPtoB * cosf(Util::Degree2Radian(theta + *parent->parentRot));
	pos.y += lenPtoB * sinf(Util::Degree2Radian(theta+ *parent->parentRot));

	sprite->SetPosition(pos);
	//�e�̉�]���u���b�N�̉�]�ɓK�p
	sprite->SetRotation(*parent->parentRot);
	sprite->MatUpdate();

}
