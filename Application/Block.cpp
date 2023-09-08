#include "Block.h"
#include"Util.h"
#include"CollisionAttribute.h"
#include<imgui_impl_dx12.h>
#include"Player.h"


//静的メンバ変数の実態
uint16_t Block::cannonTexture = 0;
uint16_t Block::blockTexture = 0;
Vector2 Block::blockSize = { 0,0 };
std::vector<std::unique_ptr<Block>> Block::pAllBlock;
Player* Block::player = nullptr;


void Block::StaticInitialize(uint16_t cannonTex, uint16_t blockTex, const Vector2& blockSize)
{
	cannonTexture = cannonTex;
	blockTexture = blockTex;
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

void Block::Initialize(const BlockData& blockData, ParentData* parent)
{
	this->blockData = blockData;
	sprite = std::make_unique<Sprite>();
	sprite->SetSize(blockSize);
	sprite->SetAnchorPoint({ 0.5f,0.5f });
	this->parent = parent;

	colManager = CollisionManager2D::GetInstance();

	//コライダーのセット
	collider = std::make_unique<CircleCollider>(Vector2{0,0},blockSize.x/2.0f);
	//属性つける
	collider->SetAttribute(COL_BLOCK);
	collider->SetSprite(sprite.get());
	//マネージャに登録
	colManager->AddCollider(collider.get());

	//ブロック識別タグの設定
	uint16_t blockTag = (uint16_t)pAllBlock.size();

	colliderTag = blockTag;
	collider->SetTag(colliderTag);


}



void Block::Draw()
{
	//データによってテクスチャ変える
	uint16_t tex = blockTexture;
	if (blockData == BlockData::None) {
		tex = cannonTexture;
	}
	sprite->Draw(tex);

}

void Block::Update()
{
	//親の座標に依存して座標移動
	if (!parent) {
		return;
	}

	Vector2 pos = *parent->parentPos;
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

	vecB.x =blockSize.x * parent->tileOffset.x;
	vecB.y =blockSize.y * parent->tileOffset.y;

	float lenPtoB = vecB.length();
	pos.x += lenPtoB * cosf(Util::Degree2Radian(theta + *parent->parentRot));
	pos.y += lenPtoB * sinf(Util::Degree2Radian(theta+ *parent->parentRot));

	sprite->SetPosition(pos);
	//親の回転をブロックの回転に適用
	sprite->SetRotation(*parent->parentRot);
	sprite->MatUpdate();

	
	ImGui::Text("tag : %d", colliderTag);
}

void Block::OnCollison()
{
	//衝突する
	if (collider->GetIsHit()) {
		//衝突対象がプレイヤー
		if (collider->GetHitCollider()->GetAttribute() == COL_PLAYER && collider->GetAttribute() == COL_BLOCK) {

			Vector2 hitOffset{ 0,0 };
			Vector2 tileOffsetBefore = parent->tileOffset;

			//プレイヤーから自分へのベクトル(プレイヤからみてどこにくっつくか判定するため、)
			Vector2 vecP = collider->GetPosition() - collider->GetHitCollider()->GetPosition();
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
					hitOffset.y = 1.0f;
				}
				else {
					//0以下なら下
					hitOffset.y = -1.0f;
				}
			}


			//自分のブロックタグ
			uint16_t baseTag = collider->GetTag();
			//ブロック総リストからどのブロックに当たったか特定
			uint16_t hitBlockTag = collider->GetHitCollider()->GetTag();
			Block* hitBlock = pAllBlock[hitBlockTag].get();
			//どのブロックに当たったかでどのピースに当たったかを特定
			uint16_t pieceTag = hitBlock->parent->parentTag;

			//親の変更
			ChangeParent(baseTag, hitBlockTag, pieceTag, hitOffset);


			//Vector2 afterOffset{ 0,0 };

			////オフセット計算式:自分の現在のオフセット - 衝突ブロックの元オフセット + (当たったブロックのオフセット + 衝突時に作成したオフセット)
			//afterOffset = (tileOffsetBefore - tileOffsetBefore) + (hitBlock->parent->tileOffset + hitOffset);

			////親を同一にする
			//parent = hitBlock->parent;
		}
	}
}

void Block::ChangeParent(uint16_t baseBlockTag, uint16_t hitBlockTag, uint16_t parentTag,const Vector2& hitOffset)
{
	//オフセット加算の基準ブロック
	Vector2 baseBlockOffset = pAllBlock[baseBlockTag]->parent->tileOffset;
	//衝突したブロック
	Vector2 hitBlockOffset = pAllBlock[hitBlockTag]->parent->tileOffset;

	//左右に当たっているのなら基準ブロックのxを反転,

	//ブロック配列走査
	for (size_t i = 0; i < pAllBlock.size(); i++) {
		//ブロックの親タグが同一なもののみ親を変える
		if (pAllBlock[i]->parent->parentTag == parentTag) {
			//ブロックの新しいオフセットを計算
			Vector2 newOffset = (hitBlockOffset + hitOffset)+ (pAllBlock[i]->parent->tileOffset -baseBlockOffset );

			//親を衝突したブロックに変更、オフセットの設定、属性の変更
			pAllBlock[i]->parent->tileOffset = newOffset;
			pAllBlock[i]->collider->SetAttribute(COL_PLAYER);

			//親の配列にぶちこむ
			player->AddBlock(pAllBlock[i].get());
		}
	}
	  


}
