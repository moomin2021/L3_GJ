#include "Boss.h"
#include "Texture.h"
#include "Util.h"

#include <imgui_impl_DX12.h>

Boss::Boss() {}

Boss::~Boss() {
}

void Boss::Initialize()
{
#pragma region �X�v���C�g
	// �{�X����0
	sBossBack0_ = std::make_unique<Sprite>();
	sBossBack0_->SetPosition(position_);
	sBossBack0_->SetRotation(rotateBossBack0_);
	sBossBack0_->SetSize({ 160.0f, 128.0f });
	sBossBack0_->SetAnchorPoint({ 0.5f, 0.5f });

	// �{�X����1
	sBossBack1_ = std::make_unique<Sprite>();
	sBossBack1_->SetPosition(position_);
	sBossBack1_->SetRotation(rotateBossBack1_);
	sBossBack1_->SetSize({ 160.0f, 128.0f });
	sBossBack1_->SetAnchorPoint({ 0.5f, 0.5f });

	// �{�X����2
	sBossFront_ = std::make_unique<Sprite>();
	sBossFront_->SetPosition(position_);
	sBossFront_->SetSize({ 128.0f, 128.0f });
	sBossFront_->SetAnchorPoint({ 0.5f, 0.5f });
#pragma endregion

#pragma region �摜�n���h��
	hBossBack_ = LoadTexture("Resources/boss_back.png");
	hBossFront_ = LoadTexture("Resources/boss_Front.png");
#pragma endregion

#pragma region �e
	// �摜�n���h����ݒ�
	BossBullet::SetHandle(LoadTexture("Resources/bullet_enemy.png"));

	// ���x��ݒ�
	BossBullet::SetSpeed(10.0f);
#pragma endregion
}

void Boss::Update()
{
	// ��ԕʍX�V����
	(this->*stateTable[state_])();

#pragma region �e�̌㏈��
	// �e�̐����t���O��[OFF]�Ȃ����
	for (auto it = bullets_.begin(); it != bullets_.end();) {
		// �e�̍X�V
		(*it)->Update();

		// �e�̐����t���O��[OFF]�ɂȂ�����e���폜
		if ((*it)->GetIsAlive() == false) it = bullets_.erase(it);
		else ++it;
	}
#pragma endregion

	ImGui::Begin("Boss");
	ImGui::Text("Position = { %f, %f }", position_.x, position_.y);
	ImGui::Text("BulletNum = %d", bullets_.size());
	ImGui::End();
}

void Boss::Draw()
{
	sBossBack0_->Draw(hBossBack_);// �{�X����0
	sBossBack1_->Draw(hBossBack_);// �{�X����1
	sBossFront_->Draw(hBossFront_);// �{�X�\��

	// �e
	for (auto& it : bullets_) {
		it->Draw();
	}
}

void Boss::OnCollision()
{
	// �e
	for (auto& it : bullets_) {
		it->OnCollision();
	}
}

void Boss::MatUpdate()
{
	sBossBack0_->MatUpdate();// �{�X����0
	sBossBack1_->MatUpdate();// �{�X����1
	sBossFront_->MatUpdate();// �{�X�\��

	// �e
	for (auto& it : bullets_) {
		it->MatUpdate();
	}
}

void (Boss::* Boss::stateTable[]) () = {
	&Boss::Wait,		// �ҋ@
	&Boss::MoveShot,	// �ړ�����
	&Boss::Summon,		// ����
	&Boss::Boomerang,	// �u�[������
};

void Boss::Wait()
{
	// �{�X�̗��ʂ���]
	rotateBossBack0_ += rotateSpd_;
	rotateBossBack1_ += rotateSpd_;
	sBossBack0_->SetRotation(rotateBossBack0_);
	sBossBack1_->SetRotation(rotateBossBack1_);
}

void Boss::MoveShot()
{
	// �{�X�̗��ʂ���]
	rotateBossBack0_ += rotateSpd_;
	rotateBossBack1_ += rotateSpd_;
	sBossBack0_->SetRotation(rotateBossBack0_);
	sBossBack1_->SetRotation(rotateBossBack1_);

#pragma region �ړ�����
	static float sin = 0.0f;
	sin += moveSpd_;
	position_.y = (sinf(sin) * 300.0f) + 540.0f;
	sBossBack0_->SetPosition(position_);
	sBossBack1_->SetPosition(position_);
	sBossFront_->SetPosition(position_);
#pragma endregion

#pragma region �e������
	// �e�������Ă���̌o�ߎ���[s]
	float elapsedTime = (Util::GetTimrMSec() - shotTime_) / 1000.0f;

	// �o�ߎ��Ԃ��C���^�[�o���ȏ�Ȃ�e������
	if (elapsedTime >= shotInterval_) {
		// ���������Ԃ�ۑ�
		shotTime_ = Util::GetTimrMSec();

		// �e�𐶐�
		bullets_.emplace_back(std::make_unique<BossBullet>());
		bullets_.back()->Initialize(sBossFront_->GetPosition());
	}
#pragma endregion
}

void Boss::Summon()
{

}

void Boss::Boomerang()
{

}