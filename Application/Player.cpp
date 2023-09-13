#include "Player.h"
#include<imgui_impl_dx12.h>
#include"Easing.h"
#include"Util.h"
#include"CollisionAttribute.h"
#include"WinAPI.h"

void Player::Initialize(uint16_t playerTexture, const Vector2& pos)
{

	//���u���b�N����
	ParentData* p = new ParentData();
	p->parentPos = pos;
	p->parentRot = 0.0f;
	p->parentTag = 0;
	p->tileOffset = { 0,0 };

	playerBlock = Block::CreateBlock(BlockData::Player, p);

	rotation = 0;

	pad = Pad::GetInstance();
	key = Key::GetInstance();

	//�̗͊֌W�̏�����
	health = healthMax;

	//�e��UI�̏�����
	spriteExpBar = std::make_unique<Sprite>();
	spriteExpBar->SetPosition({ 36.0f, 892.0f });
	spriteExpBar->SetSize({ 632.0f,40.0f });
	expBarMax = 632.0f;

	spriteExpFrame = std::make_unique<Sprite>();
	spriteExpFrame->SetPosition({352.0f, 912.0f });
	spriteExpFrame->SetSize({ 640.0f,48.0f });
	spriteExpFrame->SetAnchorPoint({ 0.5f,0.5f });

	spriteExpText = std::make_unique<Sprite>();
	spriteExpText->SetPosition({ 68.0f, 892.0f });
	spriteExpText->SetSize({ 120.0f,48.0f });
	spriteExpText->SetAnchorPoint({ 0.5f,0.5f });
	spriteExpText->SetRotation(-15.0f);

	spriteHpBar = std::make_unique<Sprite>();
	spriteHpBar->SetPosition({ 36.0f, 25.0f });
	spriteHpBar->SetSize({ 504.0f,40.0f });
	hpBarMax = 504.0f;

	spriteHpFrame = std::make_unique<Sprite>();
	spriteHpFrame->SetPosition({ 288.0f, 45.0f });
	spriteHpFrame->SetSize({ 512.0f,48.0f });
	spriteHpFrame->SetAnchorPoint({ 0.5f,0.5f });

	spriteHpText = std::make_unique<Sprite>();
	spriteHpText->SetPosition({ 58.0f, 25.0f });
	spriteHpText->SetSize({ 98.0f,48.0f });
	spriteHpText->SetAnchorPoint({ 0.5f,0.5f });
	spriteHpText->SetRotation( -15.0f);

	texExpBar = LoadTexture("Resources/exp_in.png");
	texExpFrame = LoadTexture("Resources/exp.png");
	texExpText = LoadTexture("Resources/exp_text.png");

	texHpBar = LoadTexture("Resources/hp_player_in.png");
	texHpFrame = LoadTexture("Resources/hp_player.png");
	texHpText = LoadTexture("Resources/hp_text_player.png");

}

void Player::Update()
{

	//�ړ�
	Move();

	//��]
	Rotate();

	//�U��
	Attack();

	//�e�̍X�V
	BulletsUpdate();

	//�u���b�N�𑝂₷�f�o�b�O�֐�
	AddBlock();

	//�u���b�N���Z�b�g
	BlockReset();

	/*sprite->MatUpdate();
	UpdateBlocks();*/

	// �e�̐����t���O��[OFF]�Ȃ����
	for (auto it = bullets.begin(); it != bullets.end();) {
		// �e�̍X�V
		(*it)->Update();

		// �e�̐����t���O��[OFF]�ɂȂ�����e���폜
		if ((*it)->GetIsAlive() == false) it = bullets.erase(it);
		else ++it;
	}

	//UI�X�V
	UpdateUI();

	ImGui::Text("pos %f,%f", position.x, position.y);
	ImGui::Text("health %d", health);
	ImGui::Text("Lv.%d : EXP %d", level, currentEXP);
	ImGui::SliderInt("EXP boost rate", &boostRateEXP, 1, 10);
	ImGui::SliderInt("EXP boost need Block", &countBoostEXP, 4, 32);


}

void Player::MatUpdate()
{
	playerBlock->Update();
	UpdateBlocks();

	// �e
	for (auto& it : bullets) {
		it->MatUpdate();
	}

	spriteExpBar->MatUpdate(true);
	spriteExpFrame->MatUpdate(true);
	spriteExpText->MatUpdate(true);
	spriteHpBar->MatUpdate(true);
	spriteHpFrame->MatUpdate(true);
	spriteHpText->MatUpdate(true);
}

void Player::Draw()
{
	//���@�`��
	playerBlock->Draw();

	//�u���b�N�����̕`��
	for (size_t i = 0; i < blocks.size(); i++) {
		blocks[i]->Draw();
	}

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw();
	}

}

void Player::OnCollision()
{
	////�����ɓ���������
	//if (collider->GetIsHit()) {

	//	if (collider->GetHitCollider()->GetAttribute() == COL_BLOCK) {
	//		Vector2 hitBlockOffset{ 0,0 };

	//		//�u���b�N�ɓ�������
	//		//���@����Ώۂւ̃x�N�g���쐬
	//		Vector2 vecBtoP = collider->GetHitCollider()->GetPosition() - collider->GetPosition();
	//		//x������y�����̐�Βl���r���A�c�ɂ������ɂ������߂�
	//		if (fabs(vecBtoP.x) > fabs(vecBtoP.y)) {
	//			//x�����̂ق����傫���ꍇ
	//			if (vecBtoP.x > 0) {
	//				//0�ȏ�Ȃ��ɂ���
	//				hitBlockOffset.x = 1.0f;
	//			}
	//			else {
	//				//0�ȉ��Ȃ牺
	//				hitBlockOffset.x = -1.0f;
	//			}
	//		}
	//		else {
	//			if (vecBtoP.y > 0) {
	//				//0�ȏ�Ȃ��ɂ���
	//				hitBlockOffset.y = 1.0f;
	//			}
	//			else {
	//				//0�ȉ��Ȃ牺
	//				hitBlockOffset.y = -1.0f;
	//			}
	//		}

	//		//�Փ˂����u���b�N���̂̂������ꏊ��ݒ�ł����̂ŁA�s�[�X�̃u���b�N���ׂĂɑ΂���

	//	}

	//}

	for (auto& it : bullets) {
		it->OnCollision(bulletDamage);
	}

	playerBlock->OnCollison();

	for (size_t i = 0; i < blocks.size(); i++) {
		blocks[i]->OnCollison();
	}
}

void Player::Damage(uint16_t damageValue)
{
	health -= damageValue;
	//HP���ő�l��0�ŃN�����v
	health = Util::Clamp(health, healthMax, 0);
}

void Player::DrawUI()
{
	spriteExpFrame->Draw(texExpFrame);
	spriteExpBar->Draw(texExpBar);
	spriteExpText->Draw(texExpText);
	spriteHpFrame->Draw(texHpFrame);
	spriteHpBar->Draw(texHpBar);
	spriteHpText->Draw(texHpText);

}

void Player::Move()
{
	

	//�p�b�h���͂ňړ�
	//���t���[���Ɉ��
	if (moveCoolTime > 0) {
		moveCoolTime--;
	}
	else {

		//�O�t���[���̍��W��ۑ�
		oldPos = playerBlock->GetPosition();

		Vector2 spd;
		spd = pad->GetLStick();
		//�X�e�B�b�N�̓��͂�0.5�ȏゾ������X�e�B�b�N���|����Ă��邱�Ƃɂ���
		if (fabs(spd.x) > 0.5f) {
			spd.x = 1.0f * (spd.x / (float)fabs(spd.x));
		}
		else {
			spd.x = 0;
		}

		if (fabs(spd.y) > 0.5f) {
			spd.y = 1.0f * (spd.y / (float)fabs(spd.y));
		}
		else {
			spd.y = 0;
		}

		if (spd.length() >= 1.0f) {
			spd *= baseSpd;
			moveCoolTime = moveCoolTimeMax;
		}

		//�㉺���������Ă���Ƃ�
		if (fabs(spd.x) > 1.0f && fabs(spd.y) > 1.0f) {
			//���O�̃t���[���ɏ㉺�ɓ����Ă����Ȃ獶�E��
			if (isMoveHorizontal) {
				isMoveHorizontal = false;
				spd.x = 0.0f;
			}
			else {
				isMoveHorizontal = true;
				spd.y = 0.0f;
			}
		}

		if (key->PushKey(DIK_W) || key->PushKey(DIK_A) || key->PushKey(DIK_S) || key->PushKey(DIK_D)) {
			spd.x = (key->PushKey(DIK_D) - key->PushKey(DIK_A)) * baseSpd;
			spd.y = (key->PushKey(DIK_W) - key->PushKey(DIK_S)) * baseSpd;
			moveCoolTime = moveCoolTimeMax;
		}


		spd.y = -spd.y;




		position = playerBlock->GetPosition();



		position += spd;

		//���@�̍��W�̍ő�l��ݒ�
		Vector2 size, w;
		size = Block::GetBlockSize();
		w = { (float)WinAPI::GetInstance()->GetWidth() ,(float)WinAPI::GetInstance()->GetHeight() };
		position.x = Util::Clamp(position.x, w.x - size.x / 2.0f, size.x / 2.0f);
		position.y = Util::Clamp(position.y, 
			w.y - size.y / 2.0f - (size.y*3.0f), size.y / 2.0f + size.y * 3.0f);

		ParentData* parent = playerBlock->GetParent();
		parent->parentPos = position;
		
		playerBlock->SetParent(parent);

	}

	ImGui::SliderInt("move cooltime", &moveCoolTimeMax, 1, 15);
}

void Player::Rotate()
{



	float timerate = rotEaseTime / easeTimeMax;
	//timerate�̏㉺���ݒ�
	//timerate = Util::Clamp(timerate, 1.0f, 0.0f);

	ImGui::Text("rotEaseTime %f", rotEaseTime);
	ImGui::Text("easeTimeMax %f", easeTimeMax);
	ImGui::Text("timerate %f", timerate);
	ImGui::Text("rotate %f", rotation);

	ImGui::Text("damage %d", bulletDamage);

	//timeRate��1�ȉ��Ȃ���
	if (timerate <= 1.0f) {



		rotation = beforeRot + Easing::Circ::easeOut(0.0f, childRotation, timerate);

		ParentData* parent = playerBlock->GetParent();
		parent->parentRot = rotation;

		//��]�p���ʓx�@�ɕϊ�
		playerBlock->SetParent(parent);
		rotEaseTime++;

		for (size_t i = 0; i < blocks.size(); i++) {
			ParentData* parent = blocks[i]->GetParent();
			parent->parentRot = Easing::Circ::easeOut(0.0f, childRotation, timerate);
		}
	}
	else {
		//�{�^���̃g���K�[�ŉ�]�����m
		//LB�L�[�ō���]�ARB�L�[�ŉE��]
		if (pad->GetTriggerButton(PAD_LB) || key->TriggerKey(DIK_J)) {
			beforeRot = playerBlock->GetParent()->parentRot;
			afterRot = beforeRot - 90.0f;
			rotEaseTime = 0;
		}
		else if (pad->GetTriggerButton(PAD_RB) || key->TriggerKey(DIK_K)) {
			beforeRot = playerBlock->GetParent()->parentRot;
			afterRot = beforeRot + 90.0f;
			rotEaseTime = 0;
		}

		////�q�̉�]�̑傫��
		childRotation = afterRot - beforeRot;
		//�q�̃I�t�Z�b�g�X�V(�q�̊֐����ň�x�����X�V���Ȃ��悤�ɂ���
		for (size_t i = 0; i < blocks.size(); i++) {
			blocks[i]->OffsetUpdate();
		}
	}

	//�p�x��0~360�ɂȂ�悤�ɒ���

}

void Player::Attack()
{
	//�ˌ��̃N�[���^�C�����炷
	if (shotCooltime > 0) {
		shotCooltime--;
	}
	else {
		//0�ȉ��Ȃ�N�[���^�C�����Z�b�g�AID��cannon�̃u���b�N����̂ݍU�����s��
		for (size_t i = 0; i < blocks.size(); i++) {
			if (blocks[i]->GetBlockData() == BlockData::Cannon) {
				Vector2 pos = blocks[i]->GetPosition();
				Vector2 vel(16.0f, 0.0f);
				std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();

				newBullet->Initialize(pos, vel);

				bullets.push_back(std::move(newBullet));
			}
		}

		shotCooltime = shotCooltimeMax;
	}
}

void Player::AddBlock()
{

	ImGui::Begin("add block");

	ImGui::SliderInt("offsetX", &debugBlockOffsetX, -5, 5);
	ImGui::SliderInt("offsetY", &debugBlockOffsetY, -5, 5);

	if (ImGui::Button("add")) {
		//�ݒ肳��Ă���u���b�N�̃I�t�Z�b�g���g���ău���b�N�����A���@�ƕR����
		ParentData* parent = new ParentData();
		parent->parentPos = position;
		parent->tileOffset = { (float)debugBlockOffsetX, (float)debugBlockOffsetY };
		parent->parentRot = rotation;
		Block* newBlock = Block::CreateBlock(BlockData::None, parent);

		//���@�̃u���b�N�z��Ɋi�[
		blocks.push_back(newBlock);
	}

	ImGui::End();

}

void Player::BulletsUpdate()
{
	//����ł�e������
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (!bullet->GetIsAlive()) {
			return true;
		}
		return false;
		});

	//�e�̍X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Update();
	}

}

void Player::AddBlock(Block* block)
{
	//�z��ɑ}��
	blocks.push_back(block);
}

void Player::UpdateBlocks()
{
	for (size_t i = 0; i < blocks.size(); i++) {
		//�e�̍��W�Ɖ�]�p���X�V��������
		ParentData* parent = blocks[i]->GetParent();
		parent->parentPos = position;
		//	parent->parentRot = &rotation;
		blocks[i]->SetParent(parent);
		blocks[i]->Update();

		if (blocks[i]->GetCollider()->GetAttribute() == COL_PLAYER) {
			ImGui::Text("col player");
		}

		//ImGui::Text("blocks[%d]offset:%1.f,%1.f", i, blocks[i]->GetOffset().x, blocks[i]->GetOffset().y);
	}
}

void Player::UpdateOffset()
{
}

void Player::BlockReset()
{
	//A�{�^���g���K�[�Ń��Z�b�g
	if (pad->GetTriggerButton(BUTTON::PAD_A) || key->TriggerKey(DIK_SPACE)) {
		//�z��ɂ���u���b�N�̐���ۑ�
		int blockCount = (int)blocks.size();

		//TODO:�`������Ă���`�����m���ĉ��Z����o���l�ɔ{����������
		//�u���b�N�̐����o���l�{���̕K�v���𖞂����Ă�����
		if (blockCount > countBoostEXP) {
			blockCount *= boostRateEXP;
		}


		//�o���l�ɉ��Z
		currentEXP += blockCount;

		//�z����N���A
		for (size_t i = 0; i < blocks.size(); i++) {
			blocks[i]->SetAlive(false);
		}
		blocks.clear();


		//���x���̍X�V
		LevelUpdate();
	}

}

void Player::LevelUpdate()
{
	//���݂̌o���l���K�v�o���l�𒴂��Ă����烌�x���A�b�v
	while (true)
	{
		if (currentEXP >= needEXP) {
			currentEXP -= needEXP;
			level++;
		}
		else {
			break;
		}
	}

	//�e�̃_���[�W���X�V
	bulletDamage = level;
}

void Player::UpdateUI()
{
	//�ő�HP�Ńo�[�𕪊�
	float sizeOnce = hpBarMax / (float)healthMax;

	Vector2 size = spriteHpBar->GetSize();
	size.x = sizeOnce * health;
	spriteHpBar->SetSize(size);

	//�ő�EXP�Ńo�[�𕪊�
	sizeOnce =  expBarMax / (float)needEXP;

size = spriteExpBar->GetSize();
	size.x = sizeOnce * currentEXP;
	spriteExpBar->SetSize(size);

}
