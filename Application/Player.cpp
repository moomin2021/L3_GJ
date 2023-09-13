#include "Player.h"
#include<imgui_impl_dx12.h>
#include"Easing.h"
#include"Util.h"
#include"CollisionAttribute.h"
#include"WinAPI.h"

void Player::Initialize(uint16_t playerTexture, const Vector2& pos)
{

	//自ブロック生成
	ParentData* p = new ParentData();
	p->parentPos = pos;
	p->parentRot = 0.0f;
	p->parentTag = 0;
	p->tileOffset = { 0,0 };

	playerBlock = Block::CreateBlock(BlockData::Player, p);

	rotation = 0;

	pad = Pad::GetInstance();
	key = Key::GetInstance();

	//体力関係の初期化
	health = healthMax;

	//各種UIの初期化
	spriteExpBar = std::make_unique<Sprite>();
	spriteExpBar->SetPosition({ 36.0f, 892.0f });
	spriteExpBar->SetSize({ 632.0f,40.0f });


	spriteExpFrame = std::make_unique<Sprite>();
	spriteExpFrame->SetPosition({352.0f, 912.0f });
	spriteExpFrame->SetSize({ 640.0f,48.0f });
	spriteExpFrame->SetAnchorPoint({ 0.5f,0.5f });

	spriteExpText = std::make_unique<Sprite>();
	spriteExpText->SetPosition({ 68.0f, 892.0f });
	spriteExpText->SetSize({ 120.0f,48.0f });
	spriteExpText->SetAnchorPoint({ 0.5f,0.5f });
	spriteExpText->SetRotation(-15.0f);

	spriteHpBar = std::make_unique<Sprite>();
	spriteHpBar->SetPosition({ 36.0f, 25.0f });
	spriteHpBar->SetSize({ 504.0f,40.0f });

	spriteHpFrame = std::make_unique<Sprite>();
	spriteHpFrame->SetPosition({ 288.0f, 45.0f });
	spriteHpFrame->SetSize({ 512.0f,48.0f });
	spriteHpFrame->SetAnchorPoint({ 0.5f,0.5f });

	spriteHpText = std::make_unique<Sprite>();
	spriteHpText->SetPosition({ 58.0f, 25.0f });
	spriteHpText->SetSize({ 98.0f,48.0f });
	spriteHpText->SetAnchorPoint({ 0.5f,0.5f });
	spriteHpText->SetRotation( -15.0f);

	texExpBar = LoadTexture("Resources/exp_in.png");
	texExpFrame = LoadTexture("Resources/exp.png");
	texExpText = LoadTexture("Resources/exp_text.png");

	texHpBar = LoadTexture("Resources/hp_player_in.png");
	texHpFrame = LoadTexture("Resources/hp_player.png");
	texHpText = LoadTexture("Resources/hp_text_player.png");

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

	/*sprite->MatUpdate();
	UpdateBlocks();*/

	// 弾の生存フラグが[OFF]なら消す
	for (auto it = bullets.begin(); it != bullets.end();) {
		// 弾の更新
		(*it)->Update();

		// 弾の生存フラグが[OFF]になったら弾を削除
		if ((*it)->GetIsAlive() == false) it = bullets.erase(it);
		else ++it;
	}

	ImGui::Text("pos %f,%f", position.x, position.y);
	ImGui::Text("health %d", health);
	ImGui::Text("Lv.%d : EXP %d", level, currentEXP);

}

void Player::MatUpdate()
{
	playerBlock->Update();
	UpdateBlocks();

	// 弾
	for (auto& it : bullets) {
		it->MatUpdate();
	}

	spriteExpBar->MatUpdate(true);
	spriteExpFrame->MatUpdate(true);
	spriteExpText->MatUpdate(true);
	spriteHpBar->MatUpdate(true);
	spriteHpFrame->MatUpdate(true);
	spriteHpText->MatUpdate(true);
}

void Player::Draw()
{
	//自機描画
	playerBlock->Draw();

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
	////何かに当たったら
	//if (collider->GetIsHit()) {

	//	if (collider->GetHitCollider()->GetAttribute() == COL_BLOCK) {
	//		Vector2 hitBlockOffset{ 0,0 };

	//		//ブロックに当たった
	//		//自機から対象へのベクトル作成
	//		Vector2 vecBtoP = collider->GetHitCollider()->GetPosition() - collider->GetPosition();
	//		//x成分とy成分の絶対値を比較し、縦につくか横につくか決める
	//		if (fabs(vecBtoP.x) > fabs(vecBtoP.y)) {
	//			//x成分のほうが大きい場合
	//			if (vecBtoP.x > 0) {
	//				//0以上なら上につける
	//				hitBlockOffset.x = 1.0f;
	//			}
	//			else {
	//				//0以下なら下
	//				hitBlockOffset.x = -1.0f;
	//			}
	//		}
	//		else {
	//			if (vecBtoP.y > 0) {
	//				//0以上なら上につける
	//				hitBlockOffset.y = 1.0f;
	//			}
	//			else {
	//				//0以下なら下
	//				hitBlockOffset.y = -1.0f;
	//			}
	//		}

	//		//衝突したブロック自体のくっつく場所を設定できたので、ピースのブロックすべてに対して

	//	}

	//}

	for (auto& it : bullets) {
		it->OnCollision(bulletDamage);
	}

	playerBlock->OnCollison();

	for (size_t i = 0; i < blocks.size(); i++) {
		blocks[i]->OnCollison();
	}
}

void Player::Damage(uint16_t damageValue)
{
	health -= damageValue;
	//HPを最大値と0でクランプ
	health = Util::Clamp(health, healthMax, 0);
}

void Player::DrawUI()
{
	spriteExpFrame->Draw(texExpFrame);
	spriteExpBar->Draw(texExpBar);
	spriteExpText->Draw(texExpText);
	spriteHpFrame->Draw(texHpFrame);
	spriteHpBar->Draw(texHpBar);
	spriteHpText->Draw(texHpText);

}

void Player::Move()
{
	

	//パッド入力で移動
	//数フレームに一回
	if (moveCoolTime > 0) {
		moveCoolTime--;
	}
	else {

		//前フレームの座標を保存
		oldPos = playerBlock->GetPosition();

		Vector2 spd;
		spd = pad->GetLStick();
		//スティックの入力が0.5以上だったらスティックが倒されていることにする
		if (fabs(spd.x) > 0.5f) {
			spd.x = 1.0f * (spd.x / (float)fabs(spd.x));
		}
		else {
			spd.x = 0;
		}

		if (fabs(spd.y) > 0.5f) {
			spd.y = 1.0f * (spd.y / (float)fabs(spd.y));
		}
		else {
			spd.y = 0;
		}

		if (spd.length() >= 1.0f) {
			spd *= baseSpd;
			moveCoolTime = moveCoolTimeMax;
		}

		//上下両方動いているとき
		if (fabs(spd.x) > 1.0f && fabs(spd.y) > 1.0f) {
			//直前のフレームに上下に動いていたなら左右に
			if (isMoveHorizontal) {
				isMoveHorizontal = false;
				spd.x = 0.0f;
			}
			else {
				isMoveHorizontal = true;
				spd.y = 0.0f;
			}
		}

		if (key->PushKey(DIK_W) || key->PushKey(DIK_A) || key->PushKey(DIK_S) || key->PushKey(DIK_D)) {
			spd.x = (key->PushKey(DIK_D) - key->PushKey(DIK_A)) * baseSpd;
			spd.y = (key->PushKey(DIK_W) - key->PushKey(DIK_S)) * baseSpd;
			moveCoolTime = moveCoolTimeMax;
		}


		spd.y = -spd.y;




		position = playerBlock->GetPosition();



		position += spd;

		//自機の座標の最大値を設定
		Vector2 size, w;
		size = Block::GetBlockSize();
		w = { (float)WinAPI::GetInstance()->GetWidth() ,(float)WinAPI::GetInstance()->GetHeight() };
		position.x = Util::Clamp(position.x, w.x - size.x / 2.0f, size.x / 2.0f);
		position.y = Util::Clamp(position.y, 
			w.y - size.y / 2.0f - (size.y*3.0f), size.y / 2.0f + size.y * 3.0f);

		ParentData* parent = playerBlock->GetParent();
		parent->parentPos = position;
		
		playerBlock->SetParent(parent);

	}

	ImGui::SliderInt("move cooltime", &moveCoolTimeMax, 1, 15);
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

	ImGui::Text("damage %d", bulletDamage);

	//timeRateが1以下なら補間
	if (timerate <= 1.0f) {



		rotation = beforeRot + Easing::Circ::easeOut(0.0f, childRotation, timerate);

		ParentData* parent = playerBlock->GetParent();
		parent->parentRot = rotation;

		//回転角を弧度法に変換
		playerBlock->SetParent(parent);
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
			beforeRot = playerBlock->GetParent()->parentRot;
			afterRot = beforeRot - 90.0f;
			rotEaseTime = 0;
		}
		else if (pad->GetTriggerButton(PAD_RB) || key->TriggerKey(DIK_K)) {
			beforeRot = playerBlock->GetParent()->parentRot;
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
		if (!bullet->GetIsAlive()) {
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

		if (blocks[i]->GetCollider()->GetAttribute() == COL_PLAYER) {
			ImGui::Text("col player");
		}

		//ImGui::Text("blocks[%d]offset:%1.f,%1.f", i, blocks[i]->GetOffset().x, blocks[i]->GetOffset().y);
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
		for (size_t i = 0; i < blocks.size(); i++) {
			blocks[i]->SetAlive(false);
		}
		blocks.clear();


		//レベルの更新
		LevelUpdate();
	}

}

void Player::LevelUpdate()
{
	//現在の経験値が必要経験値を超えていたらレベルアップ
	while (true)
	{
		if (currentEXP >= needEXP) {
			currentEXP -= needEXP;
			level++;
		}
		else {
			break;
		}
	}

	//弾のダメージを更新
	bulletDamage = level;
}
