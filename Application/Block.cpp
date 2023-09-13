#include "Block.h"
#include"Util.h"
#include"CollisionAttribute.h"
#include<imgui_impl_dx12.h>
#include"Player.h"
#include"Piece.h"

//�ÓI�����o�ϐ��̎���
uint16_t Block::cannonTexture = 0;
uint16_t Block::blockTexture = 0;
uint16_t Block::playerTexture = 0;
Vector2 Block::blockSize = { 0,0 };
std::vector<std::unique_ptr<Block>> Block::pAllBlock;
Player* Block::player = nullptr;
std::vector <std::unique_ptr<Piece>>* Block::pieces;
uint16_t Block::allBlockCount = 0;

void Block::StaticInitialize(uint16_t cannonTex, uint16_t blockTex, uint16_t playerTex, const Vector2& blockSize)
{
	cannonTexture = cannonTex;
	blockTexture = blockTex;
	playerTexture = playerTex;
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

void Block::SetPiece(std::vector<std::unique_ptr<Piece>>* pieces)
{
	Block::pieces = pieces;
}

void Block::AllBlockDeleteCheck()
{
	for (size_t i = 0; i < pAllBlock.size(); i++) {
		if (!pAllBlock[i]->isAlive) {
			pAllBlock.erase(pAllBlock.begin() + i);
		}
	}
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
	Vector2 size = blockSize;
	//����͎��ۂ̑傫����90����
	size *= 0.9f;

	collider = std::make_unique<BoxCollider>(Vector2{ 0,0 }, Vector2(size.x / 2.0f, size.y / 2.0f));
	//��������
	collider->SetAttribute(COL_BLOCK);
	//���ʕϐ���player�Ȃ�v���C���[��
	if (blockData == BlockData::Player) {
		collider->SetAttribute(COL_PLAYER);
	}

	collider->SetSprite(sprite.get());
	//�}�l�[�W���ɓo�^
	colManager->AddCollider(collider.get());

	//�u���b�N���ʃ^�O�̐ݒ�
	uint16_t blockTag = allBlockCount;

	colliderTag = blockTag;
	collider->SetTag(colliderTag);

	isAlive = true;
	allBlockCount++;
}



void Block::Draw()
{
	//�f�[�^�ɂ���ăe�N�X�`���ς���
	uint16_t tex = blockTexture;
	if (blockData == BlockData::None) {
		tex = cannonTexture;
	}
	else if (blockData == BlockData::Player) {
		tex = playerTexture;
	}
	sprite->Draw(tex);

}

void Block::Update()
{
	//�e�̍��W�Ɉˑ����č��W�ړ�
	if (!parent) {
		return;
	}

	Vector2 pos = parent->parentPos;

	oldPos = sprite->GetPosition();

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

	//�u���b�N�x�N�g���̑傫�����擾
	vecB.x = blockSize.x * parent->tileOffset.x;
	vecB.y = blockSize.y * parent->tileOffset.y;
	float lenPtoB = vecB.length();

	//�u���b�N�̃x�N�g���Ɗp�x�Ń��[�J�����W�v�Z
	pos.x += lenPtoB * cosf(Util::Degree2Radian(theta + parent->parentRot));
	pos.y += lenPtoB * sinf(Util::Degree2Radian(theta + parent->parentRot));


	//�e�̊p�x��90�x�Ŋ���؂�邽�тɃI�t�Z�b�g�̍X�V
	if (fabs(parent->parentRot / 90.0f) == 0.0f) {
		OffsetUpdate();
	}

	sprite->SetPosition(pos);
	//�e�̉�]���u���b�N�̉�]�ɓK�p
	sprite->SetRotation(parent->parentRot);
	sprite->MatUpdate();

	//.ImGui::Text("attribute %d", collider->GetAttribute());

	//ImGui::Text("parent tag %d", parent->parentTag);
	//ImGui::Text("offset %f,%f", parent->tileOffset.x,parent->tileOffset.y);
	//ImGui::Text("pos %f,%f", pos.x,pos.y);
	//ImGui::Text("current offset : %f,%f", currentOffset.x, currentOffset.y);
}

void Block::OffsetUpdate()
{
	//�e�̉�]��0�Ȃ�X�V���Ȃ�
	if (parent->parentRot == 0.0f) {
		return;
	}

	//�u���b�N���玩�@�ւ̃x�N�g���쐬
	Vector2 myPos = sprite->GetPosition();
	Vector2 vecBtoP = sprite->GetPosition() - parent->parentPos;
	float lenBtoP = vecBtoP.length();
	Vector2 btoPNorm = vecBtoP;
	btoPNorm.normalize();
	//�u���b�N���玩�@�ւ̃x�N�g���Ɛ��ʃx�N�g���Ŏ��@���牽�x�̌����ɂ��邩�v�Z
	Vector2 forward(1, 0);
	float theta = (forward.x * btoPNorm.x) + (forward.y * btoPNorm.y) / (forward.length()) * (btoPNorm.length());
	theta = acosf(theta);

	//�x���@�ɕϊ�
	theta = Util::Radian2Degree(theta);
	//�I�t�Z�b�g��Y���}�C�i�X�Ȃ�360�������
	if (vecBtoP.y > 0) {
		theta = 360.0f - theta;
	}

	//���W�v�Z
	Vector2 pos;
	pos.x = lenBtoP * cosf(Util::Degree2Radian(theta));
	pos.y = lenBtoP * sinf(Util::Degree2Radian(theta));

	//�u���b�N�T�C�Y�Ŋ����Č��݂̃I�t�Z�b�g�v�Z
	currentOffset.x = pos.x / blockSize.x;
	currentOffset.y = pos.y / blockSize.y;

	//�I�t�Z�b�g�X�V
	parent->tileOffset = currentOffset;

	//�e�̃I�t�Z�b�g��0�ɐݒ�
	parent->parentRot = 0.0f;

}


void Block::OnCollison()
{


	//�Փ˂���
	if (collider->GetIsHit()) {
		//�����̑������v���C���[�ŏՓˑΏۂ��u���b�N
		if (collider->GetHitCollider()->GetAttribute() == COL_BLOCK && collider->GetAttribute() == COL_PLAYER) {

			//�����̃u���b�N�^�O
			uint16_t baseTag = collider->GetTag();
			//�u���b�N�����X�g����ǂ̃u���b�N�ɓ�������������
			uint16_t hitBlockTag = collider->GetHitCollider()->GetTag();
			//���������s�[�X�̃^�O
			uint16_t pieceTag = 0;

			//�����̃I�t�Z�b�g
			Vector2 baseOffset = { 0,0 };
			//���������u���b�N�̃I�t�Z�b�g
			Vector2 hitBlockOffset = { 0,0 };

			for (size_t i = 0; i < pAllBlock.size(); i++) {

				//���������u���b�N�̃^�O����ǂ̃u���b�N�ɓ�������������
				if (hitBlockTag == pAllBlock[i]->colliderTag) {
					//�^�O������̃u���b�N�𓖂������u���b�N��
					Block* hitBlock = pAllBlock[i].get();
					//�ǂ̃u���b�N�ɓ����������łǂ̃s�[�X�ɓ��������������
					pieceTag = hitBlock->parent->parentTag;
					//���������u���b�N�̃I�t�Z�b�g���擾
					hitBlockOffset = hitBlock->parent->tileOffset;
				}
				else if (baseTag == pAllBlock[i]->colliderTag) {
					baseOffset = pAllBlock[i]->parent->tileOffset;
				}
			}





			//�Փ˃I�t�Z�b�g�v�Z

			Vector2 blockPos, playerPos, hitOffset{ 0,0 };

			//���@�̍��W���O�t���[���Ɠ���(���@�������Ă��Ȃ�)�Ȃ�G�̍��W��old
			if (oldPos == collider->GetPosition()) {
				//�u���b�N�ɑ��x�����Z���邱�Ƃŋ^���I��oldpos�����ɂ���
				blockPos = collider->GetHitCollider()->GetPosition();
				blockPos.x += blockSize.x;
				playerPos = collider->GetPosition();
			}
			else {//�O�t���[���ƈႤ�Ȃ玩���W��old
				blockPos = collider->GetHitCollider()->GetPosition();
				playerPos = oldPos;
			}


			//�v���C���[���玩���ւ̃x�N�g��(�v���C������݂Ăǂ��ɂ����������肷�邽��)
			Vector2 vecP = blockPos - playerPos;
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
					hitOffset.y = -1.0f;
				}
				else {
					//0�ȉ��Ȃ牺
					hitOffset.y = 1.0f;
				}
			}




			//�S�u���b�N����
			for (size_t i = 0; i < pAllBlock.size(); i++) {
				//�u���b�N�̐e�^�O������Ȃ��̂̂ݐe��ς���




				if (pAllBlock[i]->parent->parentTag == pieceTag && pAllBlock[i]->collider->GetAttribute() != COL_PLAYER) {


					//�u���b�N�̐V�����I�t�Z�b�g���v�Z
					Vector2 newOffset = hitOffset + baseOffset + (pAllBlock[i]->parent->tileOffset - hitBlockOffset);

					//�e���Փ˂����u���b�N�ɕύX�A�I�t�Z�b�g�̐ݒ�A�����̕ύX
					pAllBlock[i]->parent->tileOffset = newOffset;
					//*pAllBlock[i]->parent->parentRot = 0.0f;
					pAllBlock[i]->collider->SetAttribute(COL_PLAYER);

					//�e�̔z��ɂԂ�����
					player->AddBlock(pAllBlock[i].get());
				}
			}











			//�e�̕ύX
			//ChangeParent(baseTag, hitBlockTag, pieceTag, hitOffset);
		}
	}
}



Block::~Block()
{
	CollisionManager2D::GetInstance()->RemoveCollider(collider.get());
}

void Block::ChangeParent(uint16_t baseBlockTag, uint16_t hitBlockTag, uint16_t parentTag, const Vector2& hitOffset)
{
	//�I�t�Z�b�g���Z�̊�u���b�N
	Vector2 baseBlockOffset = pAllBlock[baseBlockTag]->parent->tileOffset;
	//�Փ˂����u���b�N
	Vector2 hitBlockOffset = pAllBlock[hitBlockTag]->parent->tileOffset;

	//���E�ɓ������Ă���̂Ȃ��u���b�N��x�𔽓],

	//�u���b�N�z�񑖍�
	for (size_t i = 0; i < pAllBlock.size(); i++) {
		//�u���b�N�̐e�^�O������Ȃ��̂̂ݐe��ς���
		if (pAllBlock[i]->parent->parentTag == parentTag && pAllBlock[i]->collider->GetAttribute() != COL_PLAYER) {
			//�u���b�N�̐V�����I�t�Z�b�g���v�Z
			Vector2 newOffset = (hitBlockOffset + hitOffset) + (pAllBlock[i]->parent->tileOffset - baseBlockOffset);

			//�e���Փ˂����u���b�N�ɕύX�A�I�t�Z�b�g�̐ݒ�A�����̕ύX
			pAllBlock[i]->parent->tileOffset = newOffset;
			//*pAllBlock[i]->parent->parentRot = 0.0f;
			pAllBlock[i]->collider->SetAttribute(COL_PLAYER);

			//�e�̔z��ɂԂ�����
			player->AddBlock(pAllBlock[i].get());
		}
	}

	//�s�[�X���폜
	pieces->erase(pieces->begin() + parentTag);

}
