#include "Block.h"
#include"Util.h"
#include"CollisionAttribute.h"
#include<imgui_impl_dx12.h>
#include"Player.h"
#include"Piece.h"

//静的メンバ変数の実態
uint16_t Block::cannonTexture = 0;
uint16_t Block::blockTexture = 0;
uint16_t Block::playerTexture = 0;
Vector2 Block::blockSize = { 0,0 };
std::vector<std::unique_ptr<Block>> Block::pAllBlock;
Player* Block::player = nullptr;
std::vector <std::unique_ptr<Piece>>* Block::pieces;
uint16_t Block::allBlockCount = 0;

void Block::StaticInitialize(uint16_t cannonTex, uint16_t blockTex, uint16_t playerTex, const Vector2& blockSize)
{
	cannonTexture = cannonTex;
	blockTexture = blockTex;
	playerTexture = playerTex;
	Block::blockSize = blockSize;


}

Block* Block::CreateBlock(const BlockData& blockData, ParentData* parent)
{
	std::unique_ptr<Block> newBlock = std::make_unique<Block>();
	newBlock->Initialize(blockData, parent);
	pAllBlock.push_back(std::move(newBlock));

	return pAllBlock.back().get();
}

void Block::SetPlayer(Player* player)
{
	Block::player = player;
}

void Block::SetPiece(std::vector<std::unique_ptr<Piece>>* pieces)
{
	Block::pieces = pieces;
}

void Block::AllBlockDeleteCheck()
{
	for (size_t i = 0; i < pAllBlock.size(); i++) {
		if (!pAllBlock[i]->isAlive) {
			pAllBlock.erase(pAllBlock.begin() + i);
		}
	}
}

void Block::Initialize(const BlockData& blockData, ParentData* parent)
{
	this->blockData = blockData;
	sprite = std::make_unique<Sprite>();
	sprite->SetSize(blockSize);
	sprite->SetAnchorPoint({ 0.5f,0.5f });
	this->parent = parent;

	colManager = CollisionManager2D::GetInstance();

	//コライダーのセット
	Vector2 size = blockSize;
	//判定は実際の大きさの90％に
	size *= 0.9f;

	collider = std::make_unique<BoxCollider>(Vector2{ 0,0 }, Vector2(size.x / 2.0f, size.y / 2.0f));
	//属性つける
	collider->SetAttribute(COL_BLOCK);
	//識別変数がplayerならプレイヤーに
	if (blockData == BlockData::Player) {
		collider->SetAttribute(COL_PLAYER);
	}

	collider->SetSprite(sprite.get());
	//マネージャに登録
	colManager->AddCollider(collider.get());

	//ブロック識別タグの設定
	uint16_t blockTag = allBlockCount;

	colliderTag = blockTag;
	collider->SetTag(colliderTag);

	isAlive = true;
	allBlockCount++;
}



void Block::Draw()
{
	//データによってテクスチャ変える
	uint16_t tex = blockTexture;
	if (blockData == BlockData::None) {
		tex = cannonTexture;
	}
	else if (blockData == BlockData::Player) {
		tex = playerTexture;
	}
	sprite->Draw(tex);

}

void Block::Update()
{
	//親の座標に依存して座標移動
	if (!parent) {
		return;
	}

	Vector2 pos = parent->parentPos;

	oldPos = sprite->GetPosition();

	Vector2 vecB;

	//親からのオフセットで角度を作成
	Vector2 forward = { 1,0 };
	Vector2 offset = parent->tileOffset;
	offset.normalize();

	float theta = (forward.x * offset.x) + (forward.y * offset.y) / (forward.length()) * (offset.length());
	theta = acosf(theta);

	//度数法に変換
	theta = Util::Radian2Degree(theta);
	//オフセットのYがマイナスなら360から引く
	if (offset.y > 0) {
		theta = 360.0f - theta;
	}

	//ブロックベクトルの大きさを取得
	vecB.x = blockSize.x * parent->tileOffset.x;
	vecB.y = blockSize.y * parent->tileOffset.y;
	float lenPtoB = vecB.length();

	//ブロックのベクトルと角度でローカル座標計算
	pos.x += lenPtoB * cosf(Util::Degree2Radian(theta + parent->parentRot));
	pos.y += lenPtoB * sinf(Util::Degree2Radian(theta + parent->parentRot));


	//親の角度が90度で割り切れるたびにオフセットの更新
	if (fabs(parent->parentRot / 90.0f) == 0.0f) {
		OffsetUpdate();
	}

	sprite->SetPosition(pos);
	//親の回転をブロックの回転に適用
	sprite->SetRotation(parent->parentRot);
	sprite->MatUpdate();

	//.ImGui::Text("attribute %d", collider->GetAttribute());

	//ImGui::Text("parent tag %d", parent->parentTag);
	//ImGui::Text("offset %f,%f", parent->tileOffset.x,parent->tileOffset.y);
	//ImGui::Text("pos %f,%f", pos.x,pos.y);
	//ImGui::Text("current offset : %f,%f", currentOffset.x, currentOffset.y);
}

void Block::OffsetUpdate()
{
	//親の回転が0なら更新しない
	if (parent->parentRot == 0.0f) {
		return;
	}

	//ブロックから自機へのベクトル作成
	Vector2 myPos = sprite->GetPosition();
	Vector2 vecBtoP = sprite->GetPosition() - parent->parentPos;
	float lenBtoP = vecBtoP.length();
	Vector2 btoPNorm = vecBtoP;
	btoPNorm.normalize();
	//ブロックから自機へのベクトルと正面ベクトルで自機から何度の向きにいるか計算
	Vector2 forward(1, 0);
	float theta = (forward.x * btoPNorm.x) + (forward.y * btoPNorm.y) / (forward.length()) * (btoPNorm.length());
	theta = acosf(theta);

	//度数法に変換
	theta = Util::Radian2Degree(theta);
	//オフセットのYがマイナスなら360から引く
	if (vecBtoP.y > 0) {
		theta = 360.0f - theta;
	}

	//座標計算
	Vector2 pos;
	pos.x = lenBtoP * cosf(Util::Degree2Radian(theta));
	pos.y = lenBtoP * sinf(Util::Degree2Radian(theta));

	//ブロックサイズで割って現在のオフセット計算
	currentOffset.x = pos.x / blockSize.x;
	currentOffset.y = pos.y / blockSize.y;

	//オフセット更新
	parent->tileOffset = currentOffset;

	//親のオフセットを0に設定
	parent->parentRot = 0.0f;

}


void Block::OnCollison()
{


	//衝突する
	if (collider->GetIsHit()) {
		//自分の属性がプレイヤーで衝突対象がブロック
		if (collider->GetHitCollider()->GetAttribute() == COL_BLOCK && collider->GetAttribute() == COL_PLAYER) {

			//自分のブロックタグ
			uint16_t baseTag = collider->GetTag();
			//ブロック総リストからどのブロックに当たったか特定
			uint16_t hitBlockTag = collider->GetHitCollider()->GetTag();
			//当たったピースのタグ
			uint16_t pieceTag = 0;

			//自分のオフセット
			Vector2 baseOffset = { 0,0 };
			//当たったブロックのオフセット
			Vector2 hitBlockOffset = { 0,0 };

			for (size_t i = 0; i < pAllBlock.size(); i++) {

				//当たったブロックのタグからどのブロックに当たったか特定
				if (hitBlockTag == pAllBlock[i]->colliderTag) {
					//タグが同一のブロックを当たったブロックに
					Block* hitBlock = pAllBlock[i].get();
					//どのブロックに当たったかでどのピースに当たったかを特定
					pieceTag = hitBlock->parent->parentTag;
					//当たったブロックのオフセットを取得
					hitBlockOffset = hitBlock->parent->tileOffset;
				}
				else if (baseTag == pAllBlock[i]->colliderTag) {
					baseOffset = pAllBlock[i]->parent->tileOffset;
				}
			}





			//衝突オフセット計算

			Vector2 blockPos, playerPos, hitOffset{ 0,0 };

			//自機の座標が前フレームと同じ(自機が動いていない)なら敵の座標がold
			if (oldPos == collider->GetPosition()) {
				//ブロックに速度を加算することで疑似的にoldpos扱いにする
				blockPos = collider->GetHitCollider()->GetPosition();
				blockPos.x += blockSize.x;
				playerPos = collider->GetPosition();
			}
			else {//前フレームと違うなら自座標がold
				blockPos = collider->GetHitCollider()->GetPosition();
				playerPos = oldPos;
			}


			//プレイヤーから自分へのベクトル(プレイヤからみてどこにくっつくか判定するため)
			Vector2 vecP = blockPos - playerPos;
			//x成分とy成分の絶対値を比較し、縦につくか横につくか決める
			if (fabs(vecP.x) > fabs(vecP.y)) {
				//x成分のほうが大きい場合
				if (vecP.x > 0) {
					//0以上なら上につける
					hitOffset.x = 1.0f;
				}
				else {
					//0以下なら下
					hitOffset.x = -1.0f;
				}
			}
			else {
				if (vecP.y > 0) {
					//0以上なら上につける
					hitOffset.y = -1.0f;
				}
				else {
					//0以下なら下
					hitOffset.y = 1.0f;
				}
			}




			//全ブロック走査
			for (size_t i = 0; i < pAllBlock.size(); i++) {
				//ブロックの親タグが同一なもののみ親を変える




				if (pAllBlock[i]->parent->parentTag == pieceTag && pAllBlock[i]->collider->GetAttribute() != COL_PLAYER) {


					//ブロックの新しいオフセットを計算
					Vector2 newOffset = hitOffset + baseOffset + (pAllBlock[i]->parent->tileOffset - hitBlockOffset);

					//親を衝突したブロックに変更、オフセットの設定、属性の変更
					pAllBlock[i]->parent->tileOffset = newOffset;
					//*pAllBlock[i]->parent->parentRot = 0.0f;
					pAllBlock[i]->collider->SetAttribute(COL_PLAYER);

					//親の配列にぶちこむ
					player->AddBlock(pAllBlock[i].get());
				}
			}











			//親の変更
			//ChangeParent(baseTag, hitBlockTag, pieceTag, hitOffset);
		}
	}
}



Block::~Block()
{
	CollisionManager2D::GetInstance()->RemoveCollider(collider.get());
}

void Block::ChangeParent(uint16_t baseBlockTag, uint16_t hitBlockTag, uint16_t parentTag, const Vector2& hitOffset)
{
	//オフセット加算の基準ブロック
	Vector2 baseBlockOffset = pAllBlock[baseBlockTag]->parent->tileOffset;
	//衝突したブロック
	Vector2 hitBlockOffset = pAllBlock[hitBlockTag]->parent->tileOffset;

	//左右に当たっているのなら基準ブロックのxを反転,

	//ブロック配列走査
	for (size_t i = 0; i < pAllBlock.size(); i++) {
		//ブロックの親タグが同一なもののみ親を変える
		if (pAllBlock[i]->parent->parentTag == parentTag && pAllBlock[i]->collider->GetAttribute() != COL_PLAYER) {
			//ブロックの新しいオフセットを計算
			Vector2 newOffset = (hitBlockOffset + hitOffset) + (pAllBlock[i]->parent->tileOffset - baseBlockOffset);

			//親を衝突したブロックに変更、オフセットの設定、属性の変更
			pAllBlock[i]->parent->tileOffset = newOffset;
			//*pAllBlock[i]->parent->parentRot = 0.0f;
			pAllBlock[i]->collider->SetAttribute(COL_PLAYER);

			//親の配列にぶちこむ
			player->AddBlock(pAllBlock[i].get());
		}
	}

	//ピースを削除
	pieces->erase(pieces->begin() + parentTag);

}
