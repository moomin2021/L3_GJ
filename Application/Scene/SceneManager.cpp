#include "SceneManager.h"
#include "TitleScene.h"
#include "GameScene.h"
#include "ResultScene.h"
#include "DX12Cmd.h"
#include "PipelineManager.h"
#include "ImGuiManager.h"

// �C���X�^���X�擾
SceneManager* SceneManager::GetInstance()
{
	// �֐���static�ϐ��Ƃ��Đ錾
	static SceneManager instance;
	return &instance;
}

// �R���X�g���N�^
SceneManager::SceneManager() :
#pragma region ���������X�g
	// �L�[�{�[�h���̓N���X
	key_(nullptr),

	// ���݂̃V�[��
	nowScene_(nullptr),

	// �|�X�g�G�t�F�N�g
	gaussianPostEffect_(nullptr),
	highLumiPostEffect_(nullptr),
	bloomPostEffect_(nullptr),

	// �|�X�g�G�t�F�N�g�̎��
	postEffectType_(PostEffectType::BLOOM)
#pragma endregion
{
	// �C���X�^���X�擾
	key_ = Key::GetInstance();

	// �ŏ��̃V�[��
	nowScene_ = std::make_unique<TitleScene>();
	nowScene_->Initialize();

	gaussianPostEffect_ = std::make_unique<PostEffect>();
	highLumiPostEffect_ = std::make_unique<PostEffect>();
	bloomPostEffect_ = std::make_unique<PostEffect>();

	PipelineManager::GetInstance();
}

// �f�X�g���N�^
SceneManager::~SceneManager() {

}

void SceneManager::ChangeScene(SCENE changeSceneNum)
{
	isChangeSceme_ = true;
	nextScene_ = changeSceneNum;
}

// �X�V����
void SceneManager::Update() {

	if (key_->TriggerKey(DIK_1)) postEffectType_ = PostEffectType::NORMAL;
	if (key_->TriggerKey(DIK_2)) postEffectType_ = PostEffectType::BLUR;
	if (key_->TriggerKey(DIK_3)) postEffectType_ = PostEffectType::BLOOM;

	//if (key_->TriggerKey(DIK_1)) ChangeScene(SCENE1);
	//if (key_->TriggerKey(DIK_2)) ChangeScene(SCENE2);
	//if (key_->TriggerKey(DIK_3)) ChangeScene(SCENE3);

	ImGuiManager::GetInstance()->Begin();

	nowScene_->Update();
}

// �`�揈��
void SceneManager::Draw()
{
	if (PostEffectType::NORMAL == postEffectType_) {
		// --�`��O����-- //
		DX12Cmd::GetInstance()->PreDraw();

		nowScene_->Draw();

		ImGuiManager::GetInstance()->End();
		ImGuiManager::GetInstance()->Draw();

		// --�`��㏈��-- //
		DX12Cmd::GetInstance()->PostDraw();
	}

	else if (PostEffectType::BLUR == postEffectType_) {
		gaussianPostEffect_->PreDraw();

		nowScene_->Draw();

		gaussianPostEffect_->PostDraw();

		// --�`��O����-- //
		DX12Cmd::GetInstance()->PreDraw();

		PipelineManager::GetInstance()->PreDraw("Gaussian");

		gaussianPostEffect_->Draw();

		ImGuiManager::GetInstance()->End();
		ImGuiManager::GetInstance()->Draw();

		// --�`��㏈��-- //
		DX12Cmd::GetInstance()->PostDraw();
	}

	else if (PostEffectType::BLOOM == postEffectType_) {
		highLumiPostEffect_->PreDraw();

		nowScene_->Draw();

		highLumiPostEffect_->PostDraw();

		bloomPostEffect_->PreDraw();

		PipelineManager::GetInstance()->PreDraw("HighLumi");
		highLumiPostEffect_->Draw();

		bloomPostEffect_->PostDraw();

		// --�`��O����-- //
		DX12Cmd::GetInstance()->PreDraw();

		PipelineManager::GetInstance()->PreDraw("Bloom");

		bloomPostEffect_->Draw();

		ImGuiManager::GetInstance()->End();
		//ImGuiManager::GetInstance()->Draw();

		// --�`��㏈��-- //
		DX12Cmd::GetInstance()->PostDraw();
	}

	//else if (PostEffectType::BLOOM == postEffectType_) {
	//	highLumiPostEffect_->PreDraw();

	//	nowScene_->Draw();

	//	highLumiPostEffect_->PostDraw();

	//	gaussianPostEffect_->PreDraw();

	//	PipelineManager::GetInstance()->PreDraw("HighLumi");
	//	highLumiPostEffect_->Draw();

	//	gaussianPostEffect_->PostDraw();

	//	bloomPostEffect_->PreDraw();

	//	PipelineManager::GetInstance()->PreDraw("Gaussian");

	//	gaussianPostEffect_->Draw();

	//	bloomPostEffect_->PostDraw();

	//	DX12Cmd::GetInstance()->PreDraw();

	//	bloomPostEffect_->Draw();

	//	DX12Cmd::GetInstance()->PostDraw();
	//}
}

void SceneManager::Post()
{
	if (isChangeSceme_ == false) return;

	switch (nextScene_)
	{
	case SCENE::TITLE:
		nowScene_ = std::make_unique<TitleScene>();
		nowScene_->Initialize();
		break;
	case SCENE::GAME:
		nowScene_ = std::make_unique<GameScene>();
		nowScene_->Initialize();
		break;
	case SCENE::RESULT:
		nowScene_ = std::make_unique<ResultScene>();
		nowScene_->Initialize();
		break;
	}

	isChangeSceme_ = false;
}