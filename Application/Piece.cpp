#include "Piece.h"
#include"WinAPI.h"
#include"Util.h"
#include<imgui_impl_dx12.h>

std::vector < std::unique_ptr<Piece>> Piece::pieces;

void Piece::Initialize()
{
	//àÍíUTÉ~ÉmÇÃÇ›çÏÇÈ
	/*std::unique_ptr<Block> newBlock0 = std::make_unique<Block>();
	std::unique_ptr<Block> newBlock1 = std::make_unique<Block>();
	std::unique_ptr<Block> newBlock2 = std::make_unique<Block>();
	std::unique_ptr<Block> newBlock3 = std::make_unique<Block>();*/

	uint16_t pieceTag = (uint16_t)pieces.size();

	ParentData* parent = new ParentData();
	parentPos = { (float)WinAPI::GetInstance()->GetWidth(),Util::GetRandomFloat(640,WinAPI::GetInstance()->GetHeight() - 320.0f) };


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


	//rotation = Util::GetRandomInt(0, 3) * 90.0f;

}

void Piece::Update()
{
	parentPos.x -= baseSpd;
	ImGui::Text("pos %f,%f", parentPos.x, parentPos.y);

	//ImGui::Text("piece tag :%d", pieceTag);

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

void Piece::OnCollision()
{
	for (size_t i = 0; i < childBlocks.size(); i++) {
		childBlocks[i]->OnCollison();
	}
}
