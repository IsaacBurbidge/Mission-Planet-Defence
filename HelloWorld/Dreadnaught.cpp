#include "Dreadnaught.h"
#define PLAY_IMPLEMENTATION

bool Dreadnaught::FacingRight = true;

void Dreadnaught::CreateObject(int TYPE, int WIDTH, int HEIGHT, int Collision, const char* SpriteName) {
	Play::CreateGameObject(TYPE, { WIDTH,HEIGHT }, Collision, SpriteName);
	Play::CentreSpriteOrigin(SpriteName);
}

void Dreadnaught::InstantiateDreadnaught(Point2D Pos) {
	Dreadnaught::CreateObject(MainGame::TYPE_DREADNAUGHT, Pos.x, Pos.y, 20, "Dreadnaught");
	Dreadnaught::FacingRight = true;
}

void Dreadnaught::DestroyDreadnaught(GameObject& Dreadnaught, GameObject& Bullet) {
	if (Play::IsColliding(Dreadnaught, Bullet)) {
		if (Dreadnaught::FacingRight == true) {
			Play::SetSprite(Dreadnaught, "DreadnaughtDestroy_5", 0.2f);
		}else {
			Play::SetSprite(Dreadnaught, "DreadnaughtLeftDestroy_5", 0.2f);
		}
		Play::DestroyGameObject(Bullet.GetId());
		MainGame::IncrementPoints(500);
		Play::PlayAudio("EnemyDeath");
	}
}

void Dreadnaught::DropBomb(GameObject& Dreadnaught) {
	int NewBombID = Bomb::InstantiateBomb(Dreadnaught.pos);
	GameObject& Bomb = Play::GetGameObject(NewBombID);
	Bomb::MoveBomb(Bomb);
}

void Dreadnaught::MoveDreadnaught(GameObject& Dreadnaught) {
	Dreadnaught.velocity = { 1,0 };
}

void Dreadnaught::ChangeDreadnaughtDirection(GameObject& Dreadnaught) {
	Dreadnaught.velocity = -Dreadnaught.velocity;
	if (Dreadnaught::FacingRight == true) {
		Dreadnaught::FacingRight = false;
		Play::SetSprite(Dreadnaught, "DreadnaughtLeft", 0);
	}else if (Dreadnaught::FacingRight == false) {
		Dreadnaught::FacingRight = true;
		Play::SetSprite(Dreadnaught, "Dreadnaught", 0);
	}
}