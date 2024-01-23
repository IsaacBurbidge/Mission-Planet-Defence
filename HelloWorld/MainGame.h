#pragma once
#define PLAY_USING_GAMEOBJECT_MANAGER
#include "Play.h"
class MainGame
{
public:
	enum TypeOfGameObject {
		TYPE_NULL = -1,
		TYPE_START_BUTTON,
		TYPE_EXIT_BUTTON,
		TYPE_PLAYER,
		TYPE_BULLET,
		TYPE_ENEMYBULLET,
		TYPE_ENEMY,
		TYPE_DREADNAUGHT,
		TYPE_BOMB,
		TYPE_MAINMENU_BUTTON,
		TYPE_FIGHTER,
		TYPE_BOMBANIM,
		TYPE_FIGHTER_DESTROYED,
	};
	void static IncrementPoints(int PointValue);
	void static SetAlive();
	void static DecreaseLives();
};