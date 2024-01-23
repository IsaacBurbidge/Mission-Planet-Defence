#pragma once
#include "MainGame.h"
class Enemy
{
public:
	void static InstantiateEnemy(Point2D Pos);
	void static DestroyEnemy(GameObject& Enemy, GameObject& Bullet);
	void static MoveTowardsPlayer(GameObject& Enemy, GameObject& Player);
	void static KillPlayer(GameObject& Enemy, GameObject& Player);
private:
	void static CreateEnemy(int TYPE, int WIDTH, int HEIGHT, int Collision, const char* SpriteName);
	void static StartEnemyAnim(GameObject& Enemy);
};

