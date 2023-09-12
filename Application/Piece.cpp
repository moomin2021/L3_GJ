#include "Piece.h"
#include"WinAPI.h"
#include"Util.h"
#include<imgui_impl_dx12.h>
#include"Key.h"

//静的メンバの実態
std::vector < std::unique_ptr<Piece>> Piece::pieces;

int Piece::moveCoolTimeMax = 4;
int Piece::moveCoolTime = Piece::moveCoolTimeMax;
float Piece::baseSpd = 32.0f;

void Piece::Initialize()
{
	//一旦Tミノのみ作る
	/*std::unique_ptr<Block> newBlock0 = std::make_unique<Block>();
	std::unique_ptr<Block> newBlock1 = std::make_unique<Block>();
	std::unique_ptr<Block> newBlock2 = std::make_unique<Block>();
	std::unique_ptr<Block> newBlock3 = std::make_unique<Block>();*/

	uint16_t pieceTag = (uint16_t)pieces.size();

	ParentData* parent = new ParentData();

	//縦軸のタイルの数
	int tileVertical = WinAPI::GetInstance()->GetHeight() / (int)Block::GetBlockSize().y;
	//上下のUI分減らす
	tileVertical -= 6;

	Vector2 parentOffset = { 64.0f,(float)Util::GetRandomInt(3,tileVertical) };

	parentPos.x = parentOffset.x * Block::GetBlockSize().x + (Block::GetBlockSize().x / 2.0f);
	parentPos.y = parentOffset.y * Block::GetBlockSize().y + (Block::GetBlockSize().y / 2.0f);

	parent->parentPos = parentPos;
	parent->parentRot = rotation;
	parent->parentTag = pieceTag;
	parent->tileOffset = { 0,0 };

	Block* newBlock0 = Block::CreateBlock(BlockData::None, parent);
	childBlocks.push_back(std::move(newBlock0));
	parent = new ParentData();
	parent->parentPos = parentPos;
	parent->parentRot = rotation;
	parent->parentTag = pieceTag;
	parent->tileOffset = { 0,1 };
	Block* newBlock1 = Block::CreateBlock(BlockData::Cannon, parent);
	childBlocks.push_back(std::move(newBlock1));
	parent = new ParentData();
	parent->parentPos = parentPos;
	parent->parentRot = rotation;
	parent->parentTag = pieceTag;
	parent->tileOffset = { 0,-1 };
	Block* newBlock2 = Block::CreateBlock(BlockData::None, parent);
	childBlocks.push_back(std::move(newBlock2));
	parent = new ParentData();
	parent->parentPos = parentPos;
	parent->parentRot = rotation;
	parent->parentTag = pieceTag;
	parent->tileOffset = { 1,0 };
	Block* newBlock3 = Block::CreateBlock(BlockData::Cannon, parent);
	childBlocks.push_back(std::move(newBlock3));

	myTag = pieceTag;

	//rotation = Util::GetRandomInt(0, 3) * 90.0f;

}

void Piece::Update()
{


	parentPos.x -= baseSpd;
	//parentPos.x -= baseSpd;
	//ImGui::Text("pos %f,%f", parentPos.x, parentPos.y);

	//ImGui::Text("piece tag :%d", myTag);

	for (size_t i = 0; i < childBlocks.size(); i++) {
		ParentData* parent = childBlocks[i]->GetParent();
		parent->parentPos = parentPos;
		childBlocks[i]->SetParent(parent);
		childBlocks[i]->Update();
	}
}

void Piece::Draw()
{
	for (size_t i = 0; i < childBlocks.size(); i++) {
		childBlocks[i]->Draw();
	}
}

void Piece::CreatePiece()
{

	std::unique_ptr<Piece> newPiece = std::make_unique<Piece>();
	newPiece->Initialize();
	pieces.push_back(std::move(newPiece));

}

void Piece::ALlPieceUpdate()
{
	//ピースの更新とボタンで生成
		//数フレームに1回移動
	if (moveCoolTime > 0) {
		moveCoolTime--;
	}
	else {
		moveCoolTime = moveCoolTimeMax;
		for (size_t i = 0; i < Piece::pieces.size(); i++) {
			pieces[i]->Update();
		}
	}



	ImGui::Text("piece size %d", Piece::pieces.size());

	ImGui::SliderInt("moveCoolTime", &moveCoolTimeMax, 1, 60);

	//UとIでクールタイムを増減
	if (Key::GetInstance()->TriggerKey(DIK_U)) {
		moveCoolTimeMax--;
	}
	else if (Key::GetInstance()->TriggerKey(DIK_I)) {
		moveCoolTimeMax++;
	}


	if (ImGui::Button("add piece")) {
		CreatePiece();
	}

	//ボタン押下でピース発生
	if (Key::GetInstance()->TriggerKey(DIK_P)) {
		CreatePiece();
	}
}

void Piece::OnCollision()
{
	for (size_t i = 0; i < childBlocks.size(); i++) {
		childBlocks[i]->OnCollison();
	}
}
