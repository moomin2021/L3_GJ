#pragma once
#include"Piece.h"
#include"Key.h"
#include<array>
#include<queue>

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

	void DrawNextPiece();

public:

	//next�̃s�[�X
	std::vector<PieceData> nextPiece;
	std::array<std::unique_ptr<Sprite>,4> nextSprites;
	std::array<Vector2,4> spritePos;

	//�s�[�X�z��
	std::vector < std::unique_ptr<Piece>> pieces;

	//�s�[�X�̃f�[�^�̂܂Ƃ܂�
	std::array<PieceData, AllPieceIndex> pieceIndex;

	//�s�[�X�̐i�s���x
	int moveCoolTimeMax = 10;
	int moveCoolTime = moveCoolTimeMax;
	float baseSpd = 32.0f;

	uint16_t allPieceCount = 0;

	//�s�[�X�̃X�|�[���֌W
	int pieceSpawnCoolTimeMax = 150;
	int pieceSpawnCoolTime = pieceSpawnCoolTimeMax;



private:

	void UpdateNextQuete();

	void PieceSpawn();
};

