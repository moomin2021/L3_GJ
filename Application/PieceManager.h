#pragma once
#include"Piece.h"
#include"Key.h"
#include<array>

enum PieceIndex {
	Piece_I,
	Piece_L,
	Piece_J,
	Piece_O,
	Piece_S,
	Piece_T,
	Piece_Z,
	AllPieceIndex

};

class PieceManager
{
public:
	void Initialize();

	void Update();

	void CreatePiece(const PieceData& data);

	

public:

	//ピース配列
	std::vector < std::unique_ptr<Piece>> pieces;

	//ピースのデータのまとまり
	std::array<PieceData, AllPieceIndex> pieceIndex;

	//ピースの進行速度
	int moveCoolTimeMax = 4;
	int moveCoolTime = moveCoolTimeMax;
	float baseSpd = 32.0f;

	uint16_t allPieceCount = 0;

	//ピースのスポーン関係
	int pieceSpawnCoolTimeMax = 10;
	int pieceSpawnCoolTime = pieceSpawnCoolTimeMax;

private:

	void PieceSpawn();
};

