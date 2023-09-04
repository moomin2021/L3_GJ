#include "TestPlayer.h"
#include "CollisionAttribute.h"
#include <imgui_impl_DX12.h>

TestPlayer::TestPlayer() {}

TestPlayer::~TestPlayer()
{
	colMgr_->RemoveCollider(collider_.get());
}

void TestPlayer::Initialize()
{
	// インスタンス取得
	key_ = Key::GetInstance();
	colMgr_ = CollisionManager::GetInstance();

	// モデル
	model_ = std::make_unique<Model>("sphere");

	// オブジェクト
	object_ = std::make_unique<Object3D>(model_.get());
	object_->SetPosition({ 0.0f, 0.0f, -7.0f });

	rayObj_ = std::make_unique<Object3D>(model_.get());
	rayObj_->SetPosition({ 0.0f, 0.0f, -7.0f });
	rayObj_->SetScale({ 0.1f, 0.1f, 100.0f });

	// コライダー
	collider_ = std::make_unique<RayCollider>(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{0.0f, 0.0f, 1.0f});
	collider_->SetAttribute(COL_PLAYER);
	collider_->SetObject3D(object_.get());
	colMgr_->AddCollider(collider_.get());
}

void TestPlayer::Update()
{
	// 移動
	static Vector3 pos = object_->GetPosition();

	pos.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 0.1f;
	pos.y += (key_->PushKey(DIK_SPACE) - key_->PushKey(DIK_LCONTROL)) * 0.1f;
	pos.z += (key_->PushKey(DIK_W) - key_->PushKey(DIK_S)) * 0.1f;

	object_->SetPosition(pos);
	rayObj_->SetPosition(pos);

	ImGui::Begin("Player");
	ImGui::Text("pos = {%f, %f, %f}", pos.x, pos.y, pos.z);
}

void TestPlayer::Draw()
{
	ImGui::End();

	// 描画処理
	object_->Draw();
	rayObj_->Draw();
}

void TestPlayer::OnCollision()
{
	float distance = collider_->GetDistance();
	ImGui::Text("distance = %f", distance);

	if (collider_->GetIsHit()) {
		object_->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
	}

	else {
		object_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
	}
}

void TestPlayer::ObjUpdate()
{
	// オブジェクト更新処理
	object_->Update();
	rayObj_->Update();
}