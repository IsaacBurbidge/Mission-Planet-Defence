#include "Enemy.h"
#define PLAY_IMPLEMENTATION

void Enemy::CreateEnemy(int TYPE, int WIDTH, int HEIGHT, int Collision, const char* SpriteName) {
	Play::CreateGameObject(TYPE, { WIDTH,HEIGHT }, Collision, SpriteName);
	Play::CentreSpriteOrigin(SpriteName);
}

void Enemy::InstantiateEnemy(Point2D Pos) {
	Enemy::CreateEnemy(MainGame::TYPE_ENEMY, Pos.x, Pos.y, 3, "Enemy");
}

void Enemy::StartEnemyAnim(GameObject& Enemy) {
	Play::SetSprite(Enemy, "EnemyDestroy_5", 0.2f);
	Enemy.velocity = { 0, 0 };
}

void Enemy::DestroyEnemy(GameObject& Enemy, GameObject& Bullet) {
	if (Play::IsColliding(Enemy, Bullet)) {
		Enemy::StartEnemyAnim(Enemy);
		Play::DestroyGameObject(Bullet.GetId());
		MainGame::IncrementPoints(100);
		Play::PlayAudio("EnemyDeath");
	}
}

void Enemy::MoveTowardsPlayer(GameObject& Enemy, GameObject& Player) {
	float x  = Play::RandomRollRange(-30, 30);
	float y = Play::RandomRollRange(-30, 30);
	Play::PointGameObject(Enemy, 0.1f, Player.pos.x + x , Player.pos.y + y);
}

void Enemy::KillPlayer(GameObject& Enemy, GameObject& Player) {
	if (Play::IsColliding(Enemy, Player)) {
		MainGame::DecreaseLives();
		MainGame::SetAlive();
		Enemy::StartEnemyAnim(Enemy);
		Play::PlayAudio("EnemyDeath");
	}
}
