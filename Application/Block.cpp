#include "Block.h"
#include"Util.h"
#include"CollisionAttribute.h"
#include<imgui_impl_dx12.h>
#include"Player.h"


//�ÓI�����o�ϐ��̎���
uint16_t Block::cannonTexture = 0;
uint16_t Block::blockTexture = 0;
Vector2 Block::blockSize = { 0,0 };
std::vector<std::unique_ptr<Block>> Block::pAllBlock;
Player* Block::player = nullptr;


void Block::StaticInitialize(uint16_t cannonTex, uint16_t blockTex, const Vector2& blockSize)
{
	cannonTexture = cannonTex;
	blockTexture = blockTex;
	Block::blockSize = blockSize;


}

Block* Block::CreateBlock(const BlockData& blockData, ParentData* parent)
{
	std::unique_ptr<Block> newBlock = std::make_unique<Block>();
	newBlock->Initialize(blockData, parent);
	pAllBlock.push_back(std::move(newBlock));

	return pAllBlock.back().get();
}

void Block::SetPlayer(Player* player)
{
	Block::player = player;
}

void Block::Initialize(const BlockData& blockData, ParentData* parent)
{
	this->blockData = blockData;
	sprite = std::make_unique<Sprite>();
	sprite->SetSize(blockSize);
	sprite->SetAnchorPoint({ 0.5f,0.5f });
	this->parent = parent;

	colManager = CollisionManager2D::GetInstance();

	//�R���C�_�[�̃Z�b�g
	collider = std::make_unique<CircleCollider>(Vector2{0,0},blockSize.x/2.0f);
	//��������
	collider->SetAttribute(COL_BLOCK);
	collider->SetSprite(sprite.get());
	//�}�l�[�W���ɓo�^
	colManager->AddCollider(collider.get());

	//�u���b�N���ʃ^�O�̐ݒ�
	uint16_t blockTag = (uint16_t)pAllBlock.size();

	colliderTag = blockTag;
	collider->SetTag(colliderTag);


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

	
	ImGui::Text("tag : %d", colliderTag);
}

void Block::OnCollison()
{
	//�Փ˂���
	if (collider->GetIsHit()) {
		//�ՓˑΏۂ��v���C���[
		if (collider->GetHitCollider()->GetAttribute() == COL_PLAYER && collider->GetAttribute() == COL_BLOCK) {

			Vector2 hitOffset{ 0,0 };
			Vector2 tileOffsetBefore = parent->tileOffset;

			//�v���C���[���玩���ւ̃x�N�g��(�v���C������݂Ăǂ��ɂ����������肷�邽�߁A)
			Vector2 vecP = collider->GetPosition() - collider->GetHitCollider()->GetPosition();
			//x������y�����̐�Βl���r���A�c�ɂ������ɂ������߂�
			if (fabs(vecP.x) > fabs(vecP.y)) {
				//x�����̂ق����傫���ꍇ
				if (vecP.x > 0) {
					//0�ȏ�Ȃ��ɂ���
					hitOffset.x = 1.0f;
				}
				else {
					//0�ȉ��Ȃ牺
					hitOffset.x = -1.0f;
				}
			}
			else {
				if (vecP.y > 0) {
					//0�ȏ�Ȃ��ɂ���
					hitOffset.y = 1.0f;
				}
				else {
					//0�ȉ��Ȃ牺
					hitOffset.y = -1.0f;
				}
			}


			//�����̃u���b�N�^�O
			uint16_t baseTag = collider->GetTag();
			//�u���b�N�����X�g����ǂ̃u���b�N�ɓ�������������
			uint16_t hitBlockTag = collider->GetHitCollider()->GetTag();
			Block* hitBlock = pAllBlock[hitBlockTag].get();
			//�ǂ̃u���b�N�ɓ����������łǂ̃s�[�X�ɓ��������������
			uint16_t pieceTag = hitBlock->parent->parentTag;

			//�e�̕ύX
			ChangeParent(baseTag, hitBlockTag, pieceTag, hitOffset);


			//Vector2 afterOffset{ 0,0 };

			////�I�t�Z�b�g�v�Z��:�����̌��݂̃I�t�Z�b�g - �Փ˃u���b�N�̌��I�t�Z�b�g + (���������u���b�N�̃I�t�Z�b�g + �Փˎ��ɍ쐬�����I�t�Z�b�g)
			//afterOffset = (tileOffsetBefore - tileOffsetBefore) + (hitBlock->parent->tileOffset + hitOffset);

			////�e�𓯈�ɂ���
			//parent = hitBlock->parent;
		}
	}
}

void Block::ChangeParent(uint16_t baseBlockTag, uint16_t hitBlockTag, uint16_t parentTag,const Vector2& hitOffset)
{
	//�I�t�Z�b�g���Z�̊�u���b�N
	Vector2 baseBlockOffset = pAllBlock[baseBlockTag]->parent->tileOffset;
	//�Փ˂����u���b�N
	Vector2 hitBlockOffset = pAllBlock[hitBlockTag]->parent->tileOffset;

	//���E�ɓ������Ă���̂Ȃ��u���b�N��x�𔽓],

	//�u���b�N�z�񑖍�
	for (size_t i = 0; i < pAllBlock.size(); i++) {
		//�u���b�N�̐e�^�O������Ȃ��̂̂ݐe��ς���
		if (pAllBlock[i]->parent->parentTag == parentTag) {
			//�u���b�N�̐V�����I�t�Z�b�g���v�Z
			Vector2 newOffset = (hitBlockOffset + hitOffset)+ (pAllBlock[i]->parent->tileOffset -baseBlockOffset );

			//�e���Փ˂����u���b�N�ɕύX�A�I�t�Z�b�g�̐ݒ�A�����̕ύX
			pAllBlock[i]->parent->tileOffset = newOffset;
			pAllBlock[i]->collider->SetAttribute(COL_PLAYER);

			//�e�̔z��ɂԂ�����
			player->AddBlock(pAllBlock[i].get());
		}
	}
	  


}
