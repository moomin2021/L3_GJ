#include "Player.h"
#include<imgui_impl_dx12.h>
#include"Easing.h"
#include"Util.h"
#include"CollisionAttribute.h"

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
	key = Key::GetInstance();

	colManager = CollisionManager2D::GetInstance();

	//コライダーのセット
	Vector2 size = Block::GetBlockSize();
	//判定は実際の大きさの90％に
	size *= 0.9f; 

	collider = std::make_unique<BoxCollider>(Vector2{ 0,0 }, Vector2(size.x / 2.0f, size.y / 2.0f));
	//属性つける
	collider->SetAttribute(COL_PLAYER);
	collider->SetSprite(sprite.get());
	//マネージャに登録
	colManager->AddCollider(collider.get());

	//体力関係の初期化
	health = healthMax;

}

void Player::Update()
{

	//移動
	Move();

	//回転
	Rotate();

	//攻撃
	Attack();

	//弾の更新
	BulletsUpdate();

	//ブロックを増やすデバッグ関数
	AddBlock();

	//ブロックリセット
	BlockReset();

	sprite->MatUpdate();
	UpdateBlocks();

	ImGui::Text("pos %f,%f", position.x, position.y);
	ImGui::Text("health %d", health);
	ImGui::Text("Lv.%d : EXP %d", level, currentEXP);

}

void Player::Draw()
{
	//自機描画
	sprite->Draw(texIndex);

	//ブロックたちの描画
	for (size_t i = 0; i < blocks.size(); i++) {
		blocks[i]->Draw();
	}

	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Draw();
	}

}

void Player::OnCollision()
{
	//何かに当たったら
	if (collider->GetIsHit()) {

		if (collider->GetHitCollider()->GetAttribute() == COL_BLOCK) {
			Vector2 hitBlockOffset{ 0,0 };

			//ブロックに当たった
			//自機から対象へのベクトル作成
			Vector2 vecBtoP = collider->GetHitCollider()->GetPosition() - collider->GetPosition();
			//x成分とy成分の絶対値を比較し、縦につくか横につくか決める
			if (fabs(vecBtoP.x) > fabs(vecBtoP.y)) {
				//x成分のほうが大きい場合
				if (vecBtoP.x > 0) {
					//0以上なら上につける
					hitBlockOffset.x = 1.0f;
				}
				else {
					//0以下なら下
					hitBlockOffset.x = -1.0f;
				}
			}
			else {
				if (vecBtoP.y > 0) {
					//0以上なら上につける
					hitBlockOffset.y = 1.0f;
				}
				else {
					//0以下なら下
					hitBlockOffset.y = -1.0f;
				}
			}

			//衝突したブロック自体のくっつく場所を設定できたので、ピースのブロックすべてに対して

		}

	}
}

void Player::Move()
{
	//パッド入力で移動
	Vector2 spd;
	spd = pad->GetLStick() * baseSpd;

	if (key->PushKey(DIK_W) || key->PushKey(DIK_A) || key->PushKey(DIK_S) || key->PushKey(DIK_D)) {
		spd.x = (key->PushKey(DIK_D) - key->PushKey(DIK_A)) * baseSpd;
		spd.y = (key->PushKey(DIK_W) - key->PushKey(DIK_S)) * baseSpd;
	}


	spd.y = -spd.y;

	position = sprite->GetPosition();

	position += spd;

	sprite->SetPosition(position);
}

void Player::Rotate()
{



	float timerate = rotEaseTime / easeTimeMax;
	//timerateの上下限設定
	//timerate = Util::Clamp(timerate, 1.0f, 0.0f);

	ImGui::Text("rotEaseTime %f", rotEaseTime);
	ImGui::Text("easeTimeMax %f", easeTimeMax);
	ImGui::Text("timerate %f", timerate);
	ImGui::Text("rotate %f", rotation);



	//timeRateが1以下なら補間
	if (timerate <= 1.0f) {



		rotation = beforeRot + Easing::Circ::easeOut(0.0f, childRotation, timerate);


		//回転角を弧度法に変換
		sprite->SetRotation(rotation);
		rotEaseTime++;

		for (size_t i = 0; i < blocks.size(); i++) {
			ParentData* parent = blocks[i]->GetParent();
			parent->parentRot = Easing::Circ::easeOut(0.0f, childRotation, timerate);
		}
	}
	else {
		//ボタンのトリガーで回転を検知
		//LBキーで左回転、RBキーで右回転
		if (pad->GetTriggerButton(PAD_LB) || key->TriggerKey(DIK_J)) {
			beforeRot = sprite->GetRotation();
			afterRot = beforeRot - 90.0f;
			rotEaseTime = 0;
		}
		else if (pad->GetTriggerButton(PAD_RB) || key->TriggerKey(DIK_K)) {
			beforeRot = sprite->GetRotation();
			afterRot = beforeRot + 90.0f;
			rotEaseTime = 0;
		}

		////子の回転の大きさ
		childRotation = afterRot - beforeRot;
		//子のオフセット更新(子の関数内で一度しか更新しないようにする
		for (size_t i = 0; i < blocks.size(); i++) {
			blocks[i]->OffsetUpdate();
		}
	}

	//角度が0~360になるように調整

}

void Player::Attack()
{
	//射撃のクールタイム減らす
	if (shotCooltime > 0) {
		shotCooltime--;
	}
	else {
		//0以下ならクールタイムリセット、IDがcannonのブロックからのみ攻撃を行う
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
		//設定されているブロックのオフセットを使ってブロック生成、自機と紐つける
		ParentData* parent = new ParentData();
		parent->parentPos = position;
		parent->tileOffset = { (float)debugBlockOffsetX, (float)debugBlockOffsetY };
		parent->parentRot = rotation;
		Block* newBlock = Block::CreateBlock(BlockData::None, parent);

		//自機のブロック配列に格納
		blocks.push_back(newBlock);
	}

	ImGui::End();

}

void Player::BulletsUpdate()
{
	//死んでる弾を消す
	bullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (!bullet->IsAlive()) {
			return true;
		}
		return false;
		});

	//弾の更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets) {
		bullet->Update();
	}

}

void Player::AddBlock(Block* block)
{
	//配列に挿入
	blocks.push_back(block);
}

void Player::UpdateBlocks()
{
	for (size_t i = 0; i < blocks.size(); i++) {
		//親の座標と回転角を更新し続ける
		ParentData* parent = blocks[i]->GetParent();
		parent->parentPos = position;
		//	parent->parentRot = &rotation;
		blocks[i]->SetParent(parent);
		blocks[i]->Update();
		ImGui::Text("blocks[%d]offset:%1.f,%1.f", i, blocks[i]->GetOffset().x, blocks[i]->GetOffset().y);
	}
}

void Player::UpdateOffset()
{
}

void Player::BlockReset()
{
	//Aボタントリガーでリセット
	if (pad->GetTriggerButton(BUTTON::PAD_A) || key->TriggerKey(DIK_SPACE)) {
		//配列にあるブロックの数を保存
		int blockCount = (int)blocks.size();

		//TODO:形成されている形を検知して加算する経験値に倍率をかける

		//経験値に加算
		currentEXP += blockCount;

		//配列をクリア
		blocks.clear();

	}

}
