#include "Bomb.h"
#define PLAY_IMPLEMENTATION

int Bomb::CreateObject(int TYPE, int WIDTH, int HEIGHT, int Collision, const char* SpriteName) {
	Play::CentreSpriteOrigin(SpriteName);
	return Play::CreateGameObject(TYPE, { WIDTH,HEIGHT }, Collision, SpriteName);
}

int Bomb::InstantiateBomb(Point2D Pos) {
	return Bomb::CreateObject(MainGame::TYPE_BOMB, Pos.x, Pos.y, 4, "Bomb");
}

void Bomb::StartBombAnim(GameObject& Bomb) {
	Play::PlayAudio("BombExplosion");
	Play::SetSprite(Bomb, "BombAnim_8", 0.25f);
	Bomb.velocity = { 0, 0};
}

void Bomb::MoveBomb(GameObject& Bomb) {
	Bomb.velocity = { 0,3 };
}

void Bomb::KillPlayer(GameObject& Bomb, GameObject& Player) {
	if (Play::IsColliding(Bomb, Player)) {
		MainGame::DecreaseLives();
		MainGame::SetAlive();
		Bomb::StartBombAnim(Bomb);
	}
}
