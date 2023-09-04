#include "TestEnemy.h"
#include "CollisionAttribute.h"

#include <imgui_impl_DX12.h>

TestEnemy::TestEnemy() {}

TestEnemy::~TestEnemy()
{
	colMgr_->RemoveCollider(collider_.get());
}

void TestEnemy::Initialize()
{
	// �C���X�^���X�擾
	key_ = Key::GetInstance();
	colMgr_ = CollisionManager::GetInstance();

	// ���f��
	model_ = std::make_unique<Model>("cube");

	// �I�u�W�F�N�g
	object_ = std::make_unique<Object3D>(model_.get());
	object_->SetPosition({0.0f, 5.0f, 20.0f});
	object_->SetScale({ 10.0f, 5.0f, 10.0f });

	// �R���C�_�[
	collider_ = std::make_unique<MeshCollider>(object_.get());
	collider_->SetAttribute(COL_ENEMY);
	collider_->SetObject3D(object_.get());
	colMgr_->AddCollider(collider_.get());
}

void TestEnemy::Update()
{
	// �ړ�
	static Vector3 pos = object_->GetPosition();

	pos.x += (key_->PushKey(DIK_RIGHT) - key_->PushKey(DIK_LEFT)) * 0.1f;
	pos.z += (key_->PushKey(DIK_UP) - key_->PushKey(DIK_DOWN)) * 0.1f;

	object_->SetPosition(pos);

	ImGui::Begin("Enemy");
	ImGui::Text("pos = {%f, %f, %f}", pos.x, pos.y, pos.z);
	ImGui::End();
}

void TestEnemy::Draw()
{
	// �`�揈��
	object_->Draw();
}

void TestEnemy::OnCollision()
{
	if (collider_->GetIsHit()) {
		object_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	}

	else {
		object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

void TestEnemy::ObjUpdate()
{
	// �I�u�W�F�N�g�X�V����
	object_->Update();
}