#pragma once
#include "BaseScene.h"// -> �e�V�[���̃x�[�X
#include "Key.h"// -> �L�[�{�[�h����
#include "PostEffect.h"

#include <memory>

// �V�[���񋓌^
enum SCENE {
	SCENE1,
	SCENE2,
	SCENE3,
};

enum PostEffectType {
	NORMAL,
	BLUR,
	BLOOM
};

class SceneManager {
	// --�����o�ϐ�-- //
public:

private:
	// �L�[�{�[�h���̓N���X
	Key* key_;

	// ���݂̃V�[��
	std::unique_ptr<BaseScene> nowScene_;

	// �|�X�g�G�t�F�N�g
	std::unique_ptr<PostEffect> gaussianPostEffect_;
	std::unique_ptr<PostEffect> highLumiPostEffect_;
	std::unique_ptr<PostEffect> bloomPostEffect_;

	// �|�X�g�G�t�F�N�g�̎��
	PostEffectType postEffectType_;

	// --�����o�֐�-- //
public:
	// �C���X�^���X�擾
	static SceneManager* GetInstance();

	// �X�V����
	void Update();

	// �`�揈��
	void Draw();

private:
	// �R���X�g���N�^
	SceneManager();

	// �f�X�g���N�^
	~SceneManager();

	// �V�[���؂�ւ�
	void ChangeScene(int changeSceneNum);

	// --�֎~-- //
public:
	// �R�s�[�R���X�g���N�^
	SceneManager(const SceneManager& instance) = delete;

	// ������Z�q
	SceneManager& operator=(const SceneManager& instance) = delete;
};