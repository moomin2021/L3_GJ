#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Camera.h"
#include "LightGroup.h"
#include "DirectionalLight.h"
#include "Object3D.h"
#include "ParticleEmitter.h"
#include "RayCollider.h"
#include "MeshCollider.h"
#include "SphereCollider.h"
#include "TestPlayer.h"
#include "TestEnemy.h"
#include"Player.h"
#include"Piece.h"
#include "Boss.h"

#include <memory>

class GameScene : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// �L�[�{�[�h����
	Key* key_ = nullptr;

	// �J����
	std::unique_ptr<Camera> camera_ = nullptr;

	// �v���C���[
	std::unique_ptr<Player> player = nullptr;

	// �{�X
	std::unique_ptr<Boss> boss_ = nullptr;

	//�s�[�X�z��
	std::vector < std::unique_ptr<Piece>> pieces;

	//�w�i�e�N�X�`���ƃX�v���C�g
	uint16_t backGroundTexture = 0;
	std::unique_ptr<Sprite> backGroundSprite = nullptr;

	// --�����o�֐�-- //
public:
	// �R���X�g���N�^
	GameScene();

	// �f�X�g���N�^
	~GameScene();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private:
	// �s��X�V����
	void MatUpdate();

	// �Փˎ�����
	void OnCollision();
};

