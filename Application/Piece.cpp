#include "Piece.h"
#include"WinAPI.h"
#include"Util.h"
#include<imgui_impl_dx12.h>

Piece::Piece()
{
}

Piece::~Piece()
{
	// �N���A
	blocks_.clear();
}

void Piece::Initialize(const Vector2& pos)
{
	// �������W�ݒ�
	position_ = pos;
}

void Piece::Update()
{
	// ���W�X�V
	position_.x -= moveSpd_;

	// �u���b�N�̈ʒu���C��
	for (auto& it : blocks_) {
		// �u���b�N�̃I�t�Z�b�g������W���v�Z
		Vector2 newPos = position_ + it->GetOffset() * 32.0f;
		it->SetPosition(newPos);
	}

	// �s�[�X����ʊO�ɍs�����琶���t���O��[OFF]�ɂ���
	if (position_.x <= -300.0f) {
		isAlive_ = false;
	}

	// �s�[�X�̐����t���O��[OFF]�ɂȂ�����u���b�N�̐����t���O��[OFF]�ɂ���
	if (isAlive_ == false && isCol_ == false) {
		for (auto& it : blocks_) {
			it->SetIsAlive(false);
		}
	}
}

void Piece::Draw()
{
}

void Piece::OnCollision()
{
	// �Փ˃t���O��[ON]�ɂȂ�����u���b�N�̏������v���C���[�ɕς���
	if (isCol_) {
		isAlive_ = false;
		for (auto& it : blocks_) {
			it->SetAffChangePlayer();
		}
	}
}

void Piece::MatUpdate()
{
}