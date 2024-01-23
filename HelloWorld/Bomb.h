#pragma once
#include "MainGame.h"

class Bomb
{
public:
	int static InstantiateBomb(Point2D Pos);
	void static StartBombAnim(GameObject& Bomb);
	void static MoveBomb(GameObject& Bomb);
	void static KillPlayer(GameObject& Bomb, GameObject& Player);
private:
	int static CreateObject(int TYPE, int WIDTH, int HEIGHT, int Collision, const char* SpriteName);
};