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

	//�s�[�X�z��
	std::vector < std::unique_ptr<Piece>> pieces;

	//�s�[�X�̃f�[�^�̂܂Ƃ܂�
	std::array<PieceData, AllPieceIndex> pieceIndex;

	//�s�[�X�̐i�s���x
	int moveCoolTimeMax = 4;
	int moveCoolTime = moveCoolTimeMax;
	float baseSpd = 32.0f;

	uint16_t allPieceCount = 0;

	//�s�[�X�̃X�|�[���֌W
	int pieceSpawnCoolTimeMax = 10;
	int pieceSpawnCoolTime = pieceSpawnCoolTimeMax;

private:

	void PieceSpawn();
};

