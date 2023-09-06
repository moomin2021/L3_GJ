#include "Player.h"
#include<imgui_impl_dx12.h>
#include"Easing.h"
#include"Util.h"

void Player::Initialize(uint16_t playerTexture, const Vector2& pos)
{
	texIndex = playerTexture;

	sprite = std::make_unique<Sprite>();

	//スプライトのサイズをブロックのサイズと合わせる
	sprite->SetSize(Block::GetBlockSize());
	sprite->SetPosition(pos);
	sprite->SetAnchorPoint({ 0.5f,0.5f });

	rotation = 0;

	pad = Pad::GetInstance();

	colManager = CollisionManager2D::GetInstance();

	//コライダーのセット
	collider = std::make_unique<CircleCollider>(Vector2{ 0,0 },Block::GetBlockSize().x);
	//属性つける
	//collider->SetAttribute();
	collider->SetSprite(sprite.get());
	//マネージャに登録
	colManager->AddCollider(collider.get());
}

void Player::Update()
{

	//移動
	Move();

	//回転
	Rotate();

	//ブロックを増やすデバッグ関数
	AddBlock();

	sprite->MatUpdate();
	UpdateBlocks();

}

void Player::Draw()
{
	//自機描画
	sprite->Draw(texIndex);

	//ブロックたちの描画
	for (size_t i = 0; i < blocks.size(); i++) {
		blocks[i]->Draw();
	}

}

void Player::Move()
{
	//パッド入力で移動
	Vector2 spd;
	spd = pad->GetLStick() * baseSpd;
	spd.y = -spd.y;

	position = sprite->GetPosition();

	position += spd;

	sprite->SetPosition(position);
}

void Player::Rotate()
{



	float timerate = rotEaseTime / easeTimeMax;
	//timeRateが1以下なら補間
	if (timerate < 1.0f) {
		rotation = Easing::Circ::easeOut(beforeRot, afterRot, timerate);
		//回転角を弧度法に変換
		sprite->SetRotation(rotation);
		rotEaseTime++;
	}
	else {
		//ボタンのトリガーで回転を検知
		//LBキーで左回転、RBキーで右回転
		if (pad->GetTriggerButton(PAD_LB)) {
			beforeRot = sprite->GetRotation();
			afterRot = beforeRot - 90.0f;
			rotEaseTime = 0;
		}
		else if (pad->GetTriggerButton(PAD_RB)) {
			beforeRot = sprite->GetRotation();
			afterRot = beforeRot + 90.0f;
			rotEaseTime = 0;
		}

	}
	//角度が0~360になるように調整

}

void Player::AddBlock()
{

	ImGui::Begin("add block");

	ImGui::SliderInt("offsetX", &debugBlockOffsetX, -5, 5);
	ImGui::SliderInt("offsetY", &debugBlockOffsetY, -5, 5);

	if (ImGui::Button("add")) {
		//設定されているブロックのオフセットを使ってブロック生成、自機と紐つける
		std::unique_ptr<Block> newBlock = std::make_unique<Block>();
		ParentData* parent = new ParentData();
		parent->parentPos = &position;
		parent->tileOffset = { (float)debugBlockOffsetX, (float)debugBlockOffsetY };
		parent->parentRot = &rotation;
		newBlock->Initialize(BlockData::None, parent);

		//自機のブロック配列に格納
		blocks.push_back(std::move(newBlock));
	}

	ImGui::End();

}

void Player::UpdateBlocks()
{
	for (size_t i = 0; i < blocks.size(); i++) {
		blocks[i]->Update();
	}
}
