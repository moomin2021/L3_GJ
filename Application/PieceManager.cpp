#include "PieceManager.h"
#include<imgui_impl_dx12.h>
#include"Util.h"
#include"Texture.h"
#include"WinAPI.h"

void PieceManager::Initialize()
{
	//�s�[�X���
	BlockData n = BlockData::None;
	BlockData c = BlockData::Cannon;

	pieceIndex[Piece_I].offset = { {-1,0,},{0,0},{1,0},{2,0} };
	pieceIndex[Piece_I].blockData = { n,n,c,c };
	pieceIndex[Piece_I].previewTexture = LoadTexture("Resources/piece_i.png");
	pieceIndex[Piece_I].textureSize = { 80,20 };
	pieceIndex[Piece_L].offset = { {-1,0,},{0,0},{0,1},{1,1} };
	pieceIndex[Piece_L].blockData = { c,c,n,n };
	pieceIndex[Piece_L].previewTexture = LoadTexture("Resources/piece_l.png");
	pieceIndex[Piece_L].textureSize = { 60,40 };
	pieceIndex[Piece_J].offset = { {0,-1,},{0,0},{0,1},{-1,1} };
	pieceIndex[Piece_J].blockData = { c,n,n,c };
	pieceIndex[Piece_J].previewTexture = LoadTexture("Resources/piece_j.png");
	pieceIndex[Piece_J].textureSize = { 60,40 };
	pieceIndex[Piece_O].offset = { {0,0,},{0,1},{1,0},{1,1} };
	pieceIndex[Piece_O].blockData = { n,c,c,n };
	pieceIndex[Piece_O].previewTexture = LoadTexture("Resources/piece_o.png");
	pieceIndex[Piece_O].textureSize = { 40,40 };
	pieceIndex[Piece_S].offset = { {0,-1,},{0,0},{1,0},{1,1} };
	pieceIndex[Piece_S].blockData = { c,n,n,c };
	pieceIndex[Piece_S].previewTexture = LoadTexture("Resources/piece_s.png");
	pieceIndex[Piece_S].textureSize = { 60,40 };
	pieceIndex[Piece_T].offset = { {0,-1,},{0,0},{0,1},{1,0} };
	pieceIndex[Piece_T].blockData = { c,n,n,c };
	pieceIndex[Piece_T].previewTexture = LoadTexture("Resources/piece_t.png");
	pieceIndex[Piece_T].textureSize = { 60,40 };
	pieceIndex[Piece_Z].offset = { {-1,1,},{0,1},{0,0},{1,0} };
	pieceIndex[Piece_Z].blockData = { c,n,n,c };
	pieceIndex[Piece_Z].previewTexture = LoadTexture("Resources/piece_z.png");
	pieceIndex[Piece_Z].textureSize = { 60,40 };

	//next�̂��낢��ݒ�
	for (size_t i = 0; i < nextSprites.size(); i++) {
		//�L���[�Ƀ����_���ȃf�[�^������
		uint16_t psIndexMax = (uint16_t)pieceIndex.size() - 1;
		uint16_t randomIndex = Util::GetRandomInt(0, psIndexMax);
		nextPiece.push_back(pieceIndex[randomIndex]);
		nextSprites[i] = std::make_unique<Sprite>();
		//�X�v���C�g�̃T�C�Y�̓e�N�X�`���Ɠ����ł���̂����
		nextSprites[i]->SetAnchorPoint({ 0.5f,0.5f });


		Vector2 size = Block::GetBlockSize();
		Vector2 pos = { WinAPI::GetInstance()->GetWidth() - (size.x + size.x / 2.0f),size.y + size.y / 2.0f };

		//�摜�̒��S���W��3�}�X�Ԋu��
		pos.x -= (size.x * 3) * i;

		spritePos[i] = pos;
		nextSprites[i]->SetSize(nextPiece[i].textureSize);
		nextSprites[i]->SetPosition(pos);
		nextSprites[i]->MatUpdate();




	}


}

void PieceManager::Update()
{

	//�s�[�X�̍X�V�ƃ{�^���Ő���
	//���t���[����1��ړ�
	if (moveCoolTime > 0) {
		moveCoolTime--;
	}
	else {
		moveCoolTime = moveCoolTimeMax;
		for (size_t i = 0; i < pieces.size(); i++) {
			pieces[i]->Update();
		}
	}

	//�s�[�X�����q�u���b�N�z�񂪋�Ȃ�s�[�X������
	for (size_t i = 0; i < pieces.size(); i++) {
		if (pieces[i]->GetChildCount() == 0) {
			pieces.erase(pieces.begin() + i);
		}
	}

	//�s�[�X�̎����X�|�[��
	PieceSpawn();

	ImGui::Text("piece size %d", pieces.size());

	ImGui::SliderInt("moveCoolTime", &moveCoolTimeMax, 1, 60);

	//U��I�ŃN�[���^�C���𑝌�
	if (Key::GetInstance()->TriggerKey(DIK_U)) {
		moveCoolTimeMax--;
	}
	else if (Key::GetInstance()->TriggerKey(DIK_I)) {
		moveCoolTimeMax++;
	}


	if (ImGui::Button("add piece")) {
		CreatePiece(pieceIndex[Piece_T]);
	}

	//�{�^�������Ńs�[�X����
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
		nextSprites[i]->Draw(nextPiece[i].previewTexture);
	}
}

void PieceManager::UpdateNextQuete()
{
	for (size_t i = 0; i < nextSprites.size(); i++) {
		//�X�v���C�g�̃T�C�Y�������Ă�e�N�X�`���̃T�C�Y�ƍ��킹��
		nextSprites[i]->SetSize(nextPiece[i].textureSize);
		nextSprites[i]->SetRotation(nextPiece[i].rotation);

		nextSprites[i]->MatUpdate();
	}
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

		//�L���[�̐擪������o��
		PieceData newData = nextPiece.front();

		CreatePiece(newData);

		//�L���[�̐擪���폜���A�����ɐV�����f�[�^���쐬
		nextPiece.erase(nextPiece.begin());
		PieceData newPiece = pieceIndex[randomIndex];
		//���̃s�[�X�̉�]�������_���Ɍ���
		//newPiece.rotation = (float)Util::GetRandomInt(0, 3) * 90.0f;
		nextPiece.push_back(newPiece);


		UpdateNextQuete();
	}

}
