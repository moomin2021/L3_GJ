#include "PieceManager.h"
#include<imgui_impl_dx12.h>
#include"Util.h"
#include"Texture.h"
#include"WinAPI.h"

void PieceManager::Initialize()
{
	//ピース作る
	BlockData n = BlockData::None;
	BlockData c = BlockData::Cannon;

	pieceIndex[Piece_I].offset = { {0,-1,},{0,0},{0,1},{0,2} };
	pieceIndex[Piece_I].blockData = { n,n,c,c };
	pieceIndex[Piece_I].previewTexture = LoadTexture("Resources/piece_i.png");
	pieceIndex[Piece_L].offset = { {-1,0,},{0,0},{0,1},{0,2} };
	pieceIndex[Piece_L].blockData = { c,c,n,n };
	pieceIndex[Piece_I].previewTexture = LoadTexture("Resources/piece_l.png");
	pieceIndex[Piece_J].offset = { {0,-1,},{0,0},{0,1},{1,1} };
	pieceIndex[Piece_J].blockData = { c,c,n,n };
	pieceIndex[Piece_I].previewTexture = LoadTexture("Resources/piece_j.png");
	pieceIndex[Piece_O].offset = { {0,0,},{0,1},{1,0},{1,1} };
	pieceIndex[Piece_O].blockData = { n,c,c,n };
	pieceIndex[Piece_I].previewTexture = LoadTexture("Resources/piece_o.png");
	pieceIndex[Piece_S].offset = { {0,-1,},{0,0},{1,0},{1,1} };
	pieceIndex[Piece_S].blockData = { c,n,n,c };
	pieceIndex[Piece_I].previewTexture = LoadTexture("Resources/piece_s.png");
	pieceIndex[Piece_T].offset = { {0,-1,},{0,0},{0,1},{1,0} };
	pieceIndex[Piece_T].blockData = { c,n,n,c };
	pieceIndex[Piece_I].previewTexture = LoadTexture("Resources/piece_t.png");
	pieceIndex[Piece_Z].offset = { {-1,-1,},{-1,0},{0,0},{0,1} };
	pieceIndex[Piece_Z].blockData = { c,n,n,c };
	pieceIndex[Piece_I].previewTexture = LoadTexture("Resources/piece_z.png");

	//nextのいろいろ設定
	for (size_t i = 0; i < nextSprites.size(); i++) {
		//キューにランダムなデータを入れる
		uint16_t psIndexMax = (uint16_t)pieceIndex.size() - 1;
		uint16_t randomIndex = Util::GetRandomInt(0, psIndexMax);
		nextPiece.push(pieceIndex[randomIndex]);
		nextSprites[i] = std::make_unique<Sprite>();
		//スプライトのサイズはテクスチャと同期できるのか問題
		nextSprites[i]->SetAnchorPoint({ 0.5f,0.5f });

		Vector2 size = Block::GetBlockSize();
		Vector2 pos = { WinAPI::GetInstance()->GetWidth() - (size.x + size.x / 2.0f),size.y + size.y / 2.0f };

		pos.x -= (size.x * 2) * i;

		spritePos.push_back(pos);

	}


}

void PieceManager::Update()
{

	//ピースの更新とボタンで生成
	//数フレームに1回移動
	if (moveCoolTime > 0) {
		moveCoolTime--;
	}
	else {
		moveCoolTime = moveCoolTimeMax;
		for (size_t i = 0; i < pieces.size(); i++) {
			pieces[i]->Update();
		}
	}

	//ピースの自動スポーン
	PieceSpawn();

	ImGui::Text("piece size %d", pieces.size());

	ImGui::SliderInt("moveCoolTime", &moveCoolTimeMax, 1, 60);

	//UとIでクールタイムを増減
	if (Key::GetInstance()->TriggerKey(DIK_U)) {
		moveCoolTimeMax--;
	}
	else if (Key::GetInstance()->TriggerKey(DIK_I)) {
		moveCoolTimeMax++;
	}


	if (ImGui::Button("add piece")) {
		CreatePiece(pieceIndex[Piece_T]);
	}

	//ボタン押下でピース発生
	if (Key::GetInstance()->TriggerKey(DIK_P)) {
		CreatePiece(pieceIndex[Piece_T]);
	}

}

void PieceManager::CreatePiece(const PieceData& data)
{
	std::unique_ptr<Piece> newPiece = std::make_unique<Piece>();
	newPiece->Initialize(data);
	pieces.push_back(std::move(newPiece));
}

void PieceManager::DrawNextPiece()
{
	for (size_t i = 0; i < nextSprites.size(); i++) {
		//nextSprites[i]->Draw(nextPiece.)
	}
}

void PieceManager::UpdateNextQuete()
{

}

void PieceManager::PieceSpawn()
{
	ImGui::SliderInt("piece cooltime", &pieceSpawnCoolTimeMax, 1, 60);


	if (pieceSpawnCoolTime > 0) {
		pieceSpawnCoolTime--;
	}
	else {
		pieceSpawnCoolTime = pieceSpawnCoolTimeMax;

		uint16_t psIndexMax = (uint16_t)pieceIndex.size() - 1;
		uint16_t randomIndex = Util::GetRandomInt(0, psIndexMax);

		CreatePiece(pieceIndex[randomIndex]);



	}

}
