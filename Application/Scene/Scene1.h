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

#include <memory>

class Scene1 : public BaseScene
{
	// --�����o�ϐ�-- //
private:
	// �L�[�{�[�h����
	Key* key_ = nullptr;

	// �J����
	std::unique_ptr<Camera> camera_ = nullptr;

	// ���C�g�O���[�v
	std::unique_ptr<LightGroup> lightGroup_ = nullptr;

	// ���s����
	std::unique_ptr<DirectionalLight> dirLight_ = nullptr;

	// BGM�L�[
	uint16_t bgmKey_ = 0;

	// �v���C���[
	std::unique_ptr<TestPlayer> player_ = nullptr;
	std::unique_ptr<Player> player = nullptr;

	// �G�l�~�[
	std::unique_ptr<TestEnemy> enemy_ = nullptr;

	//�s�[�X�z��
	std::vector < std::unique_ptr<Piece>> pieces;

	// --�����o�֐�-- //
public:
	// �R���X�g���N�^
	Scene1();

	// �f�X�g���N�^
	~Scene1();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private:
	// �I�u�W�F�N�g�X�V����
	void ObjUpdate();

	// �Փˎ�����
	void OnCollision();
};

