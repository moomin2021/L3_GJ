#include "Boss.h"
#include "Texture.h"
#include "Util.h"
#include "Easing.h"
#include "PipelineManager.h"

#include <imgui_impl_DX12.h>

Boss::Boss() {}

Boss::~Boss() {
}

void Boss::Initialize()
{
#pragma region �C���X�^���X�擾
	key_ = Key::GetInstance();
#pragma endregion

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
	hParticle_ = LoadTexture("Resources/particle_enemy.png");
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

#pragma region �p�[�e�B�N���G�~�b�^�[
	emitterBack0_ = std::make_unique<ParticleEmitter2D>(120);
	emitterBack1_ = std::make_unique<ParticleEmitter2D>(120);
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

#pragma region �G���G�̌㏈��
	// �G���G�̐����t���O��[OFF]�Ȃ����
	for (auto it = enemys_.begin(); it != enemys_.end();) {
		// �G���G�̍X�V
		(*it)->Update();

		// �G���G�̐����t���O��[OFF]�ɂȂ�����e���폜
		if ((*it)->GetIsAlive() == false) it = enemys_.erase(it);
		else ++it;
	}
#pragma endregion

	// �f�o�b�N�p
	DebugImGui();
}

void Boss::Draw()
{
	PipelineManager::PreDraw("Sprite");

	sBossBack0_->Draw(hBossBack_);// �{�X����0
	sBossBack1_->Draw(hBossBack_);// �{�X����1
	sBossFront_->Draw(hBossFront_);// �{�X�\��

	// �e
	for (auto& it : bullets_) {
		it->Draw();
	}

	// �G���G
	for (auto& it : enemys_) {
		it->Draw();
	}

	PipelineManager::PreDraw("Particle2D", D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	emitterBack0_->Draw(hParticle_);
	emitterBack1_->Draw(hParticle_);
}

void Boss::OnCollision()
{
	// �e
	for (auto& it : bullets_) {
		it->OnCollision();
	}

	// �G���G
	for (auto& it : enemys_) {
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

	// �G���G
	for (auto& it : enemys_) {
		it->MatUpdate();
	}

#pragma region �p�[�e�B�N���G�~�b�^�[
	emitterBack0_->Update();
	emitterBack1_->Update();
#pragma endregion
}

void (Boss::* Boss::stateTable[]) () = {
	&Boss::Wait,			// �ҋ@
	&Boss::PreMoveShot,		// �ړ������O����
	&Boss::MoveShot,		// �ړ�����
	&Boss::PostMoveShot,	// �ړ������㏈��
	&Boss::PreSummon,		// �����O����
	&Boss::Summon,			// ����
	&Boss::PostSummon,		// �����㏈��
	&Boss::PreBoomerang,	// �u�[�������O����
	&Boss::Boomerang,		// �u�[������
	&Boss::PostBoomerang,	// �u�[�������㏈��
};

void Boss::Wait()
{
	// �{�X�̗��ʉ�]
	BossBackRotate(basicSpd_);
}

void Boss::PreMoveShot()
{
#pragma region �J�ڑO���W�����{���W�܂ňړ�
	// �s���J�n����̌o�ߎ���
	float elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	// �o�ߎ��Ԃ̊����ňړ�
	float rate = Util::Clamp(elapsedTime / time2PreMoveShot_, 1.0f, 0.0f);

	// �o�ߎ��Ԃ��w�莞�Ԉȏ�Ȃ�State��Summon�ɂ���
	if (elapsedTime >= time2PreMoveShot_) {
		state_ = MOVE_SHOT;
		sinMove_ = 0.0f;
		actionStartTime_ = Util::GetTimrMSec();
	}
#pragma endregion

#pragma region ���ʉ�]
	// �{�X�̗��ʉ�]
	BossBackRotate(Easing::Quint::easeOut(basicSpd_, moveShotRotaSpd_, rate));
#pragma endregion
}

void Boss::MoveShot()
{
	// �{�X�̗��ʉ�]
	BossBackRotate(moveShotRotaSpd_);

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

#pragma region �ړ��������ԊǗ�
	// �ړ��������J�n���Ă���w��̎��Ԃ��o���Ă����珢�����I��
	elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	if (elapsedTime >= time2MoveShot_) {
		state_ = POST_MOVE_SHOT;
		actionStartTime_ = Util::GetTimrMSec();
		beforePos_ = position_;
	}
#pragma endregion
}

void Boss::PostMoveShot()
{
#pragma region �J�ڑO���W�����{���W�܂ňړ�
	// �s���J�n����̌o�ߎ���
	float elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	// �o�ߎ��Ԃ̊����ňړ�
	float rate = Util::Clamp(elapsedTime / time2PostMoveShot_, 1.0f, 0.0f);
	position_.x = Easing::Quint::easeOut(beforePos_.x, basicPos_.x, rate);
	position_.y = Easing::Quint::easeOut(beforePos_.y, basicPos_.y, rate);
	backPos0_ = position_;
	backPos1_ = position_;

	// �o�ߎ��Ԃ��w�莞�Ԉȏ�Ȃ�State��WAIT�ɂ���
	if (elapsedTime >= time2PostMoveShot_) {
		state_ = WAIT;
		actionStartTime_ = Util::GetTimrMSec();
	}
#pragma endregion

#pragma region ���ʉ�]
	// �{�X�̗��ʉ�]
	BossBackRotate(Easing::Quint::easeOut(moveShotRotaSpd_, basicSpd_, rate));
#pragma endregion

#pragma region �X�v���C�g�f�[�^�X�V
	sBossBack0_->SetPosition(backPos0_);
	sBossBack1_->SetPosition(backPos1_);
	sBossFront_->SetPosition(position_);
#pragma endregion
}

void Boss::PreSummon()
{
#pragma region �J�ڑO���W�����{���W�܂ňړ�
	// �s���J�n����̌o�ߎ���
	float elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	// �o�ߎ��Ԃ̊����ňړ�
	float rate = Util::Clamp(elapsedTime / time2PreSummon_, 1.0f, 0.0f);

	// �o�ߎ��Ԃ��w�莞�Ԉȏ�Ȃ�State��Summon�ɂ���
	if (elapsedTime >= time2PreSummon_) {
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

#pragma region �p�[�e�B�N��
	float rnd0X = Util::GetRandomFloat(0.0f, 1.0f) * 2.0f - 1.0f;
	float rnd0Y = Util::GetRandomFloat(0.0f, 1.0f) * 2.0f - 1.0f;
	emitterBack0_->Add(60, { rnd0X * 10.0f, rnd0Y * 10.0f }, { 3.0f * rnd0X, 3.0f * rnd0Y }, { 0.05f * -rnd0X, 0.05f * rnd0Y }, 32.0f, 0.0f);
	emitterBack0_->SetPosition(backPos0_);

	float rnd1X = Util::GetRandomFloat(0.0f, 1.0f) * 2.0f - 1.0f;
	float rnd1Y = Util::GetRandomFloat(0.0f, 1.0f) * 2.0f - 1.0f;
	emitterBack1_->Add(60, { rnd1X * 10.0f, rnd1Y * 10.0f }, { 3.0f * rnd1X, 3.0f * rnd1Y }, { 0.05f * -rnd1X, 0.05f * rnd1Y }, 32.0f, 0.0f);
	emitterBack1_->SetPosition(backPos1_);
#pragma endregion

#pragma region sin�֐��ňړ�����
	sinMove_ += sinSpd_;
	backPos0_.y = (sinf(sinMove_) * 300.0f) + 540.0f;;
	backPos1_.y = (-sinf(sinMove_) * 300.0f) + 540.0f;;
#pragma endregion

#pragma region �G���G����
	// �e�������Ă���̌o�ߎ���[s]
	float elapsedTime = (Util::GetTimrMSec() - summonTime_) / 1000.0f;

	// �o�ߎ��Ԃ��C���^�[�o���ȏ�Ȃ珢������
	if (elapsedTime >= summonInterval_) {
		// �����������Ԃ�ۑ�
		summonTime_ = Util::GetTimrMSec();

		// �G�𐶐�
		enemys_.emplace_back(std::make_unique<Enemy0>());
		enemys_.back()->Initialize(backPos0_);
		enemys_.emplace_back(std::make_unique<Enemy0>());
		enemys_.back()->Initialize(backPos1_);
	}
#pragma endregion

#pragma region �������ԊǗ�
	// �������Ă���w��̎��Ԃ��o���Ă����珢�����I��
	elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	if (elapsedTime >= time2Summon_) {
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
	float rate = Util::Clamp(elapsedTime / time2PostSummon_, 1.0f, 0.0f);
	backPos0_.x = Easing::Quint::easeOut(beforeBackPos0_.x, basicPos_.x, rate);
	backPos0_.y = Easing::Quint::easeOut(beforeBackPos0_.y, basicPos_.y, rate);
	backPos1_.x = Easing::Quint::easeOut(beforeBackPos1_.x, basicPos_.x, rate);
	backPos1_.y = Easing::Quint::easeOut(beforeBackPos1_.y, basicPos_.y, rate);

	// �o�ߎ��Ԃ��w�莞�Ԉȏ�Ȃ�State��Summon�ɂ���
	if (elapsedTime >= time2PostSummon_) {
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

void Boss::PreBoomerang()
{
#pragma region �J�ڑO���W�����{���W�܂ňړ�
	// �s���J�n����̌o�ߎ���
	float elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	// �o�ߎ��Ԃ̊����ňړ�
	float rate = Util::Clamp(elapsedTime / time2PreBoomerang_, 1.0f, 0.0f);

	// �o�ߎ��Ԃ��w�莞�Ԉȏ�Ȃ�State��Boomerang�ɂ���
	if (elapsedTime >= time2PreBoomerang_) {
		state_ = BOOMERANG;
		actionStartTime_ = Util::GetTimrMSec();
	}

	// �o�ߎ��Ԃ̊����ňړ�
	backPos0_.x = Easing::Quint::easeOut(basicPos_.x, 1500.0f, rate);
	backPos0_.y = Easing::Quint::easeOut(basicPos_.y, 840.0f, rate);
	backPos1_.x = Easing::Quint::easeOut(basicPos_.x, 1500.0f, rate);
	backPos1_.y = Easing::Quint::easeOut(basicPos_.y, 840.0f, rate);
#pragma endregion

#pragma region ���ʉ�]
	// �{�X�̗��ʉ�]
	BossBackRotate(Easing::Quint::easeOut(basicSpd_, boomerangRotaSpd_, rate));
#pragma endregion

#pragma region �X�v���C�g�f�[�^�X�V
	sBossBack0_->SetPosition(backPos0_);
	sBossBack1_->SetPosition(backPos1_);
	sBossFront_->SetPosition(position_);
#pragma endregion
}

void Boss::Boomerang()
{
	if (isBoomerang_ == false) {
		backPos0_.x -= 30.0f;
		backPos1_.x -= 30.0f;

		if (backPos0_.x <= -300.0f) {
			isBoomerang_ = true;
			backPos0_.y = 240.0f;
			backPos1_.y = 240.0f;
		}
	}

	else {
		backPos0_.x += 30.0f;
		backPos1_.x += 30.0f;

		if (backPos0_.x >= 1500.0f) {
			isBoomerang_ = false;
			state_ = POST_BOOMERANG;
			actionStartTime_ = Util::GetTimrMSec();
			beforeBackPos0_ = backPos0_;
			beforeBackPos1_ = backPos1_;
		}
	}

#pragma region �p�[�e�B�N��
	for (size_t i = 0; i < 2; i++) {
		float rnd0X = Util::GetRandomFloat(0.0f, 360.0f);
		float rnd0Y = Util::GetRandomFloat(0.0f, 360.0f);
		Vector2 result0 = { cosf(rnd0X), sinf(rnd0Y) };
		result0.normalize();
		emitterBack0_->Add(60, { result0.x * 70.0f, result0.y * 70.0f }, { result0.x * 1.5f, result0.y * 1.5f }, { 0.0f, 0.0f }, 32.0f, 0.0f);
		emitterBack0_->SetPosition(backPos0_);

		float rnd1X = Util::GetRandomFloat(-360.0f, 0.0f);
		float rnd1Y = Util::GetRandomFloat(-360.0f, 0.0f);
		Vector2 result1 = { cosf(rnd1X), sinf(rnd1Y) };
		result1.normalize();
		emitterBack1_->Add(60, { result1.x * 70.0f, result1.y * 70.0f }, { result1.x * 1.5f, result1.y * 1.5f }, { 0.0f, 0.0f }, 32.0f, 0.0f);
		emitterBack1_->SetPosition(backPos1_);
	}
#pragma endregion

#pragma region ���ʉ�]
	// �{�X�̗��ʉ�]
	BossBackRotate(boomerangRotaSpd_);
#pragma endregion

#pragma region �X�v���C�g�f�[�^�X�V
	sBossBack0_->SetPosition(backPos0_);
	sBossBack1_->SetPosition(backPos1_);
#pragma endregion
}

void Boss::PostBoomerang()
{
#pragma region �J�ڑO���W�����{���W�܂ňړ�
	// �s���J�n����̌o�ߎ���
	float elapsedTime = (Util::GetTimrMSec() - actionStartTime_) / 1000.0f;

	// �o�ߎ��Ԃ̊����ňړ�
	float rate = Util::Clamp(elapsedTime / time2PostBoomerang_, 1.0f, 0.0f);
	backPos0_.x = Easing::Quint::easeOut(beforeBackPos0_.x, basicPos_.x, rate);
	backPos0_.y = Easing::Quint::easeOut(beforeBackPos0_.y, basicPos_.y, rate);
	backPos1_.x = Easing::Quint::easeOut(beforeBackPos1_.x, basicPos_.x, rate);
	backPos1_.y = Easing::Quint::easeOut(beforeBackPos1_.y, basicPos_.y, rate);

	// �o�ߎ��Ԃ��w�莞�Ԉȏ�Ȃ�State��WAIT�ɂ���
	if (elapsedTime >= time2PostBoomerang_) {
		state_ = WAIT;
		actionStartTime_ = Util::GetTimrMSec();
	}
#pragma endregion

#pragma region ���ʉ�]
	// �{�X�̗��ʉ�]
	BossBackRotate(Easing::Quint::easeOut(boomerangRotaSpd_, basicSpd_, rate));
#pragma endregion

#pragma region �X�v���C�g�f�[�^�X�V
	sBossBack0_->SetPosition(backPos0_);
	sBossBack1_->SetPosition(backPos1_);
#pragma endregion
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
	//ImGui::Begin("Boss");
	//ImGui::Text("State = %s", stateText_[state_].c_str());
	//ImGui::Text("Position = { %f, %f }", position_.x, position_.y);
	//ImGui::Text("BackPos0 = { %f, %f }", backPos0_.x, backPos0_.y);
	//ImGui::Text("BackPos1 = { %f, %f }", backPos1_.x, backPos1_.y);
	//if (ImGui::Button("Start Sumoon") && state_ == WAIT) {
	//	state_ = PRE_SUMMON;
	//	actionStartTime_ = Util::GetTimrMSec();
	//}

	//if (ImGui::Button("Start MoveShot") && state_ == WAIT) {
	//	state_ = PRE_MOVE_SHOT;
	//	actionStartTime_ = Util::GetTimrMSec();
	//}

	//if (ImGui::Button("Start Boomerang") && state_ == WAIT) {
	//	state_ = PRE_BOOMERANG;
	//	actionStartTime_ = Util::GetTimrMSec();
	//}
	//ImGui::End();

	if (key_->TriggerKey(DIK_8) && state_ == WAIT) {
		state_ = PRE_SUMMON;
		actionStartTime_ = Util::GetTimrMSec();
	}

	if (key_->TriggerKey(DIK_9) && state_ == WAIT) {
		state_ = PRE_MOVE_SHOT;
		actionStartTime_ = Util::GetTimrMSec();
	}

	if (key_->TriggerKey(DIK_0) && state_ == WAIT) {
		state_ = PRE_BOOMERANG;
		actionStartTime_ = Util::GetTimrMSec();
	}
}