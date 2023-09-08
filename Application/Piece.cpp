#include "Piece.h"
#include"WinAPI.h"
#include"Util.h"
#include<imgui_impl_dx12.h>

std::vector < std::unique_ptr<Piece>> Piece::pieces;

void Piece::Initialize()
{
	//一旦Tミノのみ作る
	std::unique_ptr<Block> newBlock0 = std::make_unique<Block>();
	std::unique_ptr<Block> newBlock1 = std::make_unique<Block>();
	std::unique_ptr<Block> newBlock2 = std::make_unique<Block>();
	std::unique_ptr<Block> newBlock3 = std::make_unique<Block>();

	ParentData* parent = new ParentData();
	parent->parentPos = &parentPos;
	parent->parentRot = &rotation;
	parent->tileOffset = { 0,0 };
	parent->parentTag = pieceTag;
	newBlock0->Initialize(BlockData::Cannon, parent);


	childBlocks.push_back(std::move(newBlock0));
	parent = new ParentData();
	parent->parentPos = &parentPos;
	parent->parentRot = &rotation;
	parent->tileOffset = { 0,1 };
	parent->parentTag = pieceTag;
	newBlock1->Initialize(BlockData::None, parent);
	childBlocks.push_back(std::move(newBlock1));
	parent = new ParentData();
	parent->parentPos = &parentPos;
	parent->parentRot = &rotation;
	parent->tileOffset = { 0,-1 };
	parent->parentTag = pieceTag;
	newBlock2->Initialize(BlockData::None, parent);
	childBlocks.push_back(std::move(newBlock2));
	parent = new ParentData();
	parent->parentPos = &parentPos;
	parent->parentRot = &rotation;
	parent->tileOffset = { 1,0 };
	parent->parentTag = pieceTag;
	newBlock3->Initialize(BlockData::Cannon, parent);
	childBlocks.push_back(std::move(newBlock3));

	parentPos = { (float)WinAPI::GetInstance()->GetWidth(),Util::GetRandomFloat(64,WinAPI::GetInstance()->GetHeight() - 64.0f) };

	rotation = Util::GetRandomInt(0, 3) * 90.0f;

}

void Piece::Update()
{
	parentPos.x -= baseSpd;

	ImGui::Text("tag :%d", pieceTag);

	for (size_t i = 0; i < childBlocks.size(); i++) {
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
	if (ImGui::Button("add piece")) {
		std::unique_ptr<Piece> newPiece = std::make_unique<Piece>();
		newPiece->Initialize();
		newPiece->pieceTag = (uint16_t)pieces.size();
		pieces.push_back(std::move(newPiece));
	}
}

void Piece::OnCollision()
{
	for (size_t i = 0; i < childBlocks.size(); i++) {
		childBlocks[i]->OnCollison();
	}
}
