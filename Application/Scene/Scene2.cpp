#include "Scene2.h"
#include "Texture.h"
#include "Vector3.h"
#include "PipelineManager.h"
#include "Sound.h"
#include "CollisionManager2D.h"
#include "CollisionAttribute.h"
#include "ImGuiManager.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_DX12.h>

Scene2::Scene2() :
	key_(nullptr)
{
}

Scene2::~Scene2()
{
	CollisionManager2D::GetInstance()->RemoveCollider(collider_[0].get());
	CollisionManager2D::GetInstance()->RemoveCollider(collider_[1].get());
}

void Scene2::Initialize()
{
	// キーボード入力インスタンス取得
	key_ = Key::GetInstance();

	// カメラ
	camera_ = std::make_unique<Camera>();
	camera_->SetEye({ 0.0f, 1.0f, -20.0f });
	Sprite::SetCamera(camera_.get());

#pragma region スプライト
	sprite_[0] = std::make_unique<Sprite>();
	sprite_[0]->SetPosition({ 100.0f, 100.0f });

	sprite_[1] = std::make_unique<Sprite>();
	sprite_[1]->SetPosition({ 300.0f, 100.0f });
#pragma endregion

#pragma region コライダー
	collider_[0] = std::make_unique<BoxCollider>();
	collider_[0]->SetRadius({ 50.0f, 50.0f });
	collider_[0]->SetSprite(sprite_[0].get());
	collider_[0]->SetAttribute(COL_PLAYER);
	CollisionManager2D::GetInstance()->AddCollider(collider_[0].get());

	collider_[1] = std::make_unique<BoxCollider>();
	collider_[1]->SetRadius({ 50.0f, 50.0f });
	collider_[1]->SetSprite(sprite_[1].get());
	collider_[1]->SetAttribute(COL_PLAYER);
	CollisionManager2D::GetInstance()->AddCollider(collider_[1].get());
#pragma endregion
}

void Scene2::Update()
{
	static Vector2 pos = { 100.0f, 100.0f };
	pos.x += (key_->PushKey(DIK_D) - key_->PushKey(DIK_A)) * 1.0f;
	pos.y += (key_->PushKey(DIK_S) - key_->PushKey(DIK_W)) * 1.0f;

	sprite_[0]->SetPosition(pos);

	// 衝突時処理
	OnCollision();

	// 行列更新
	MatUpdate();
}

void Scene2::Draw()
{
	PipelineManager::PreDraw("Sprite");

	for (auto& it : sprite_) {
		it->Draw();
	}
}

void Scene2::OnCollision()
{
	CollisionManager2D::GetInstance()->CheckAllCollision();

	for (size_t i = 0; i < 2;i++) {
		if (collider_[i]->GetIsHit()) {
			sprite_[i]->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
		}

		else {
			sprite_[i]->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
		}
	}
}

void Scene2::MatUpdate()
{
	camera_->Update();

	for (auto& it : sprite_) {
		it->MatUpdate();
	}
}