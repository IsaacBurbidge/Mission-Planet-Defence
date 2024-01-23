#pragma once
#include "MainGame.h"
#include "Bullet.h"
class Fighter
{
public:
	void static InstantiateFighter(Point2D Pos);
	void static InstantiateFighter2(Point2D Pos);
	void static DestroyFighter(GameObject& Enemy, GameObject& Bullet);
	void static FireBullet(GameObject& Fighter, bool IsLeftFighter);
	void static MoveFighter(GameObject& Fighter);
private:
	void static CreateObject(int TYPE, int WIDTH, int HEIGHT, int Collision, const char* SpriteName);
	void static StartFighterAnim(GameObject& Fighter);
	void static StartFighter2Anim(GameObject& Fighter);
};