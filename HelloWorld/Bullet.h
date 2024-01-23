#pragma once
#include "MainGame.h"
class Bullet
{
public:
	void static InstantiatePlayerBullet(Point2D Pos);
	int static InstantiateEnemyBullet(Point2D Pos);
	void static MoveBulletRight(GameObject& Bullet);
	void static MoveBulletLeft(GameObject& Bullet);
	void static KillPlayer(GameObject& Bullet, GameObject& Player);
private:
	void static CreateBullet(int TYPE, int WIDTH, int HEIGHT, int Collision, const char* SpriteName);
	int static CreateEnemyBullet(int TYPE, int WIDTH, int HEIGHT, int Collision, const char* SpriteName);
};

