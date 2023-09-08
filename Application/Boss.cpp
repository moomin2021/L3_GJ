#include "Boss.h"
#include "Texture.h"

Boss::Boss() {}

Boss::~Boss() {}

void Boss::Initialize()
{
#pragma region �X�v���C�g
	// �{�X����0
	sBossBack0_ = std::make_unique<Sprite>();
	sBossBack0_->SetPosition(position_);
	sBossBack0_->SetRotation(rotateBossBack0_);
	sBossBack0_->SetSize({ 128.0f, 128.0f });
	sBossBack0_->SetAnchorPoint({ 0.5f, 0.5f });

	// �{�X����1
	sBossBack1_ = std::make_unique<Sprite>();
	sBossBack1_->SetPosition(position_);
	sBossBack1_->SetRotation(rotateBossBack1_);
	sBossBack1_->SetSize({ 128.0f, 128.0f });
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
}

void Boss::Update()
{
	// ��ԕʍX�V����
	(this->*stateTable[state_])();
}

void Boss::Draw()
{
	sBossBack0_->Draw(hBossBack_);// �{�X����0
	sBossBack1_->Draw(hBossBack_);// �{�X����1
	sBossFront_->Draw(hBossFront_);// �{�X�\��
}

void Boss::OnCollision()
{
}

void Boss::MatUpdate()
{
	sBossBack0_->MatUpdate();// �{�X����0
	sBossBack1_->MatUpdate();// �{�X����1
	sBossFront_->MatUpdate();// �{�X�\��
}

void (Boss::* Boss::stateTable[]) () = {
	&Boss::Wait,		// �ҋ@
	&Boss::MoveShot,	// �ړ�����
	&Boss::Summon,		// ����
	&Boss::Boomerang,	// �u�[������
};

void Boss::Wait()
{

}

void Boss::MoveShot()
{

}

void Boss::Summon()
{

}

void Boss::Boomerang()
{

}