#pragma once
#include "BaseScene.h"
#include "Key.h"
#include "Camera.h"
#include "LightGroup.h"
#include "DirectionalLight.h"
#include "Object3D.h"
#include "RayCollider.h"
#include "MeshCollider.h"
#include "SphereCollider.h"
#include"Player.h"
#include"Piece.h"
#include "Boss.h"
#include "UIManager.h"
#include "ParticleEmitter2D.h"
#include"PieceManager.h"
#include "Fade.h"
#include "Sound.h"

#include <memory>

class GameScene : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// �L�[�{�[�h����
	Key* key_ = nullptr;
	Sound* sound_ = nullptr;

	// �T�E���h�n���h��
	uint16_t gameBGM_ = 0;

	// �t�F�[�h
	std::unique_ptr<Fade> fade_ = nullptr;

	// �J����
	std::unique_ptr<Camera> camera_ = nullptr;

	// �v���C���[
	std::unique_ptr<Player> player = nullptr;

	// �{�X
	std::unique_ptr<Boss> boss_ = nullptr;

	//�s�[�X�Ǘ��N���X
	std::unique_ptr<PieceManager> psManager = nullptr;

	//�w�i�e�N�X�`���ƃX�v���C�g
	uint16_t backGroundTexture = 0;
	std::unique_ptr<Sprite> backGroundSprite = nullptr;

	// UI�}�l�[�W���[
	std::unique_ptr<UIManager> uiMgr_ = nullptr;

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

