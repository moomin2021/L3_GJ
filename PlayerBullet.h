#pragma once
#include"sprite.h"
#include<memory>

class PlayerBullet
{
public:

	void Initialize(const Vector2& pos, const Vector2& velocity);

	void Update();

	void Draw();

	bool IsAlive()const { return isAlive; }

private:
	static Vector2 bulletSize;
	static uint16_t bulletTex;

	Vector2 pos = { 0,0 };

	std::unique_ptr<Sprite> sprite = nullptr;
	Vector2 vel{ 0,0 };
	bool isAlive = true;

};

