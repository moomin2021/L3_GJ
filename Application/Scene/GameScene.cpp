#include "GameScene.h"

GameScene::GameScene() {}

GameScene::~GameScene() {}

void GameScene::Initialize()
{
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

}

void GameScene::OnCollision()
{

}

void GameScene::ObjUpdate()
{

}