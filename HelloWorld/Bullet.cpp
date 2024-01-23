#include "Bullet.h"
#define PLAY_IMPLEMENTATION

void Bullet::CreateBullet(int TYPE, int WIDTH, int HEIGHT, int Collision, const char* SpriteName) {
	Play::CreateGameObject(TYPE, { WIDTH,HEIGHT }, Collision, SpriteName);
	Play::CentreSpriteOrigin(SpriteName);
}

void Bullet::InstantiatePlayerBullet(Point2D Pos){
	Bullet::CreateBullet(MainGame::TYPE_BULLET, Pos.x, Pos.y, 3, "PlayerBullet");
}

int Bullet::CreateEnemyBullet(int TYPE, int WIDTH, int HEIGHT, int Collision, const char* SpriteName) {
	Play::CentreSpriteOrigin(SpriteName);
	return Play::CreateGameObject(TYPE, { WIDTH,HEIGHT }, Collision, SpriteName);
}

int Bullet::InstantiateEnemyBullet(Point2D Pos) {
	return Bullet::CreateEnemyBullet(MainGame::TYPE_ENEMYBULLET, Pos.x, Pos.y, 3, "EnemyBullet");
}

void Bullet::MoveBulletRight(GameObject& Bullet) {
	Bullet.velocity = { 2,0 };
}

void Bullet::MoveBulletLeft(GameObject& Bullet) {
	Bullet.velocity = { -2,0 };
}

void Bullet::KillPlayer(GameObject& Bullet, GameObject& Player) {
	if (Play::IsColliding(Bullet, Player)) {
		MainGame::DecreaseLives();
		MainGame::SetAlive();
		Play::DestroyGameObject(Bullet.GetId());
	}
}
