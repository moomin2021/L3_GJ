#include "Scene3.h"
#include "Texture.h"
#include "PipelineManager.h"
#include "CollisionAttribute.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_DX12.h>

Scene3::Scene3() {}

Scene3::~Scene3()
{
	colMgr2D_->RemoveCollider(pCol_.get());
	colMgr2D_->RemoveCollider(boxCol_.get());
}

void Scene3::Initialize()
{
	// キーボード入力インスタンス取得
	key_ = Key::GetInstance();
	mouse_ = Mouse::GetInstance();

	colMgr2D_ = CollisionManager2D::GetInstance();

#pragma region スプライト初期化
	sBox_ = std::make_unique<Sprite>();
	sBox_->SetPosition(Vector2{ 500.0f, 300.0f });
	sBox_->SetSize(Vector2{ 100.0f, 100.0f });
	sBox_->SetAnchorPoint(Vector2{ 0.5f, 0.5f });
#pragma endregion

#pragma region コライダー初期化
	pCol_ = std::make_unique<PointCollider>(Vector2{ 0.0f, 0.0f });
	pCol_->SetAttribute(COL_POINT);
	colMgr2D_->AddCollider(pCol_.get());

	boxCol_ = std::make_unique<BoxCollider>(Vector2{ 500.0f, 300.0f }, Vector2{ 50.0f, 50.0f });
	boxCol_->SetAttribute(COL_BOX);
	colMgr2D_->AddCollider(boxCol_.get());
#pragma endregion
}

void Scene3::Update()
{
	// コライダーの座標を設定
	pCol_->SetPosition(mouse_->MousePos());

	// 衝突判定
	OnCollision();

	// オブジェクト更新
	ObjUpdate();

	Vector2 pos = pCol_->GetPosition();

	ImGui::Begin("Debug");
	ImGui::Text("mouse = {%f, %f}", pos.x, pos.y);
	ImGui::End();
}

void Scene3::Draw()
{
	PipelineManager::PreDraw("Sprite");

	sBox_->Draw();
}

void Scene3::OnCollision()
{
	// 衝突判定
	CollisionManager2D::GetInstance()->CheckAllCollision();

	if (boxCol_->GetIsHit()) sBox_->SetColor(float4{ 1.0f, 0.0f, 0.0f, 1.0f });
	else sBox_->SetColor(float4{ 1.0f, 1.0f, 1.0f, 1.0f });
}

void Scene3::ObjUpdate()
{

}