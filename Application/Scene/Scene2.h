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

#include <memory>

class Scene2 : public BaseScene
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

	// ���f��
	std::unique_ptr<Model> model_ = nullptr;

	// �I�u�W�F�N�g
	std::unique_ptr<Object3D> object_ = nullptr;

	// �p�[�e�B�N���G�~�b�^�[
	std::vector<std::unique_ptr<ParticleEmitter>> particleEmitters_;

	// ���C�R���C�_�[
	std::unique_ptr<RayCollider> rayCol_ = nullptr;

	// ���b�V���R���C�_�[
	std::unique_ptr<MeshCollider> meshCol_ = nullptr;

	// ���R���C�_�[
	std::unique_ptr<SphereCollider> sphereCol_ = nullptr;

	// �p�[�e�B�N���p�摜�n���h��
	uint16_t particlehandle_ = 0;

	// BGM�L�[
	uint16_t bgmKey_ = 0;

	// --�����o�֐�-- //
public:
	// �R���X�g���N�^
	Scene2();

	// �f�X�g���N�^
	~Scene2();

	// ����������
	void Initialize();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();
};

