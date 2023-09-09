#include "Boss.h"
#include "Texture.h"
#include "Util.h"
#include "Easing.h"

#include <imgui_impl_DX12.h>

Boss::Boss() {}

Boss::~Boss() {
}

void Boss::Initialize()
{
#pragma region �X�v���C�g
	// �{�X����0
	sBossBack0_ = std::make_unique<Sprite>();
	sBossBack0_->SetPosition(backPos0_);
	sBossBack0_->SetRotation(rotateBossBack0_);
	sBossBack0_->SetSize({ 160.0f, 160.0f });
	sBossBack0_->SetAnchorPoint({ 0.5f, 0.5f });

	// �{�X����1
	sBossBack1_ = std::make_unique<Sprite>();
	sBossBack1_->SetPosition(backPos1_);
	sBossBack1_->SetRotation(rotateBossBack1_);
	sBossBack1_->SetSize({ 160.0f, 160.0f });
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

#pragma region BossBullet
	// �摜�n���h����ݒ�
	BossBullet::SetHandle(LoadTexture("Resources/bullet_enemy.png"));

	// ���x��ݒ�
	BossBullet::SetSpeed(10.0f);
#pragma endregion

#pragma region Enemy0
	Enemy0::SetBackHandle(LoadTexture("Resources/enemy_back.png"));
	Enemy0::SetFrontHandle(LoadTexture("Resources/enemy_front.png"));
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

	// �f�o�b�N�p
	DebugImGui();
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
	&Boss::PreSummon,	// �����O����
	&Boss::Summon,		// ����
	&Boss::PostSummon,	// �����㏈��
	&Boss::Boomerang,	// �u�[������
};

void Boss::Wait()
{
	// �{�X�̗��ʉ�]
	BossBackRotate(basicSpd_);
}

void Boss::MoveShot()
{
	// �{�X�̗��ʉ�]
	BossBackRotate(basicSpd_);

#pragma region sin�֐��ňړ�����
	sinMove_ += sinSpd_;
	position_.y = (sinf(sinMove_) * 300.0f) + 540.0f;
	backPos0_ = position_;
	backPos1_ = position_;
	sBossBack0_->SetPosition(backPos0_);
	sBossBack1_->SetPosition(backPos1_);
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

void Boss::PreSummon()
{
#pragma region �J�ڑO���W�����{���W�܂ňړ�
	// �s���J�n����̌o�ߎ���
	float elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	// �o�ߎ��Ԃ̊����ňړ�
	float rate = Util::Clamp(elapsedTime / preSummonTime_, 1.0f, 0.0f);
	//position_.x = Easing::Quint::easeOut(beforePos_.x, basicPos_.x, rate);
	//position_.y = Easing::Quint::easeOut(beforePos_.y, basicPos_.y, rate);
	//backPos0_ = position_;
	//backPos1_ = position_;

	// �o�ߎ��Ԃ��w�莞�Ԉȏ�Ȃ�State��Summon�ɂ���
	if (elapsedTime >= preSummonTime_) {
		state_ = SUMMON;
		sinMove_ = 0.0f;
		actionStartTime_ = Util::GetTimrMSec();
	}
#pragma endregion

#pragma region ���ʉ�]
	// �{�X�̗��ʉ�]
	BossBackRotate(Easing::Quint::easeOut(basicSpd_, summonRotaSpd_, rate));
#pragma endregion

#pragma region �X�v���C�g�f�[�^�X�V
	sBossBack0_->SetPosition(backPos0_);
	sBossBack1_->SetPosition(backPos1_);
	sBossFront_->SetPosition(position_);
#pragma endregion
}

void Boss::Summon()
{
	// �{�X�̗��ʉ�]
	BossBackRotate(summonRotaSpd_);

#pragma region sin�֐��ňړ�����
	sinMove_ += sinSpd_;
	backPos0_.y = (sinf(sinMove_) * 300.0f) + 540.0f;;
	backPos1_.y = (-sinf(sinMove_) * 300.0f) + 540.0f;;
#pragma endregion

#pragma region �������ԊǗ�
	// �������Ă���w��̎��Ԃ��o���Ă����珢�����I��
	float elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	if (elapsedTime >= summonTime_) {
		state_ = POST_SUMMON;
		actionStartTime_ = Util::GetTimrMSec();
		beforeBackPos0_ = backPos0_;
		beforeBackPos1_ = backPos1_;
	}
#pragma endregion

#pragma region �X�v���C�g�f�[�^�X�V
	sBossBack0_->SetPosition(backPos0_);
	sBossBack1_->SetPosition(backPos1_);
#pragma endregion
}

void Boss::PostSummon()
{

#pragma region �J�ڑO���W�����{���W�܂ňړ�
	// �s���J�n����̌o�ߎ���
	float elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	// �o�ߎ��Ԃ̊����ňړ�
	float rate = Util::Clamp(elapsedTime / postSummonTime_, 1.0f, 0.0f);
	backPos0_.x = Easing::Quint::easeOut(beforeBackPos0_.x, basicPos_.x, rate);
	backPos0_.y = Easing::Quint::easeOut(beforeBackPos0_.y, basicPos_.y, rate);
	backPos1_.x = Easing::Quint::easeOut(beforeBackPos1_.x, basicPos_.x, rate);
	backPos1_.y = Easing::Quint::easeOut(beforeBackPos1_.y, basicPos_.y, rate);

	// �o�ߎ��Ԃ��w�莞�Ԉȏ�Ȃ�State��Summon�ɂ���
	if (elapsedTime >= postSummonTime_) {
		state_ = WAIT;
		actionStartTime_ = Util::GetTimrMSec();
	}
#pragma endregion

#pragma region ���ʉ�]
	// �{�X�̗��ʉ�]
	BossBackRotate(Easing::Quint::easeOut(summonRotaSpd_, basicSpd_, rate));
#pragma endregion

#pragma region �X�v���C�g�f�[�^�X�V
	sBossBack0_->SetPosition(backPos0_);
	sBossBack1_->SetPosition(backPos1_);
#pragma endregion
}

void Boss::Boomerang()
{

}

void Boss::BossBackRotate(float rotate)
{
	rotateBossBack0_ += rotate;
	rotateBossBack1_ += rotate;
	rotateBossFront_ -= basicSpd_;
	sBossBack0_->SetRotation(rotateBossBack0_);
	sBossBack1_->SetRotation(rotateBossBack1_);
	sBossFront_->SetRotation(rotateBossFront_);
}

void Boss::DebugImGui()
{
	ImGui::Begin("Boss");
	ImGui::Text("State = %s", stateText_[state_].c_str());
	ImGui::Text("Position = { %f, %f }", position_.x, position_.y);
	ImGui::Text("BackPos0 = { %f, %f }", backPos0_.x, backPos0_.y);
	ImGui::Text("BackPos1 = { %f, %f }", backPos1_.x, backPos1_.y);
	if (ImGui::Button("Start Sumoon")) {
		state_ = PRE_SUMMON;
		actionStartTime_ = Util::GetTimrMSec();
	}
	ImGui::End();
}