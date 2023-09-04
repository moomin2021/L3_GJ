#include "GameScene.h"
#include "PipelineManager.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize()
{
#pragma region インスタンス
	key_ = Key::GetInstance();// キーボード入力
#pragma endregion
}

void GameScene::Update()
{
	// 衝突時処理
	OnCollision();

	// オブジェクト処理
	ObjUpdate();
}

void GameScene::Draw()
{
	// スプライト前準備
	PipelineManager::GetInstance()->PreDraw("Sprite");
}

void GameScene::OnCollision()
{

}

void GameScene::ObjUpdate()
{

}