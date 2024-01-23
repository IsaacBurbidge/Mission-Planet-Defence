#pragma once
#include "MainGame.h"
#include "Bomb.h"
class Dreadnaught
{
public:
	void static InstantiateDreadnaught(Point2D Pos);
	void static DestroyDreadnaught(GameObject& Dreadnaught, GameObject& Bullet);
	void static DropBomb(GameObject& Dreadnaught);
	void static MoveDreadnaught(GameObject& Dreadnaught);
	void static ChangeDreadnaughtDirection(GameObject& Dreadnaught);
	static bool FacingRight;
private:
	void static CreateObject(int TYPE, int WIDTH, int HEIGHT, int Collision, const char* SpriteName);
};
