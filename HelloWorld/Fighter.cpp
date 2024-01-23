#include "Fighter.h"
#define PLAY_IMPLEMENTATION

void Fighter::CreateObject(int TYPE, int WIDTH, int HEIGHT, int Collision, const char* SpriteName) {
	Play::CreateGameObject(TYPE, { WIDTH,HEIGHT }, Collision, SpriteName);
	Play::CentreSpriteOrigin(SpriteName);
}

void Fighter::InstantiateFighter(Point2D Pos) {
	Fighter::CreateObject(MainGame::TYPE_FIGHTER, Pos.x, Pos.y, 20, "Fighter");
}

void Fighter::InstantiateFighter2(Point2D Pos) {
	Fighter::CreateObject(MainGame::TYPE_FIGHTER, Pos.x, Pos.y, 20, "FighterRight");
}

void Fighter::StartFighterAnim(GameObject& Fighter) {
	Play::SetSprite(Fighter, "FighterDestroy_5", 0.2f);
	Fighter.type = MainGame::TYPE_FIGHTER_DESTROYED;
	Fighter.velocity = { 0, 0 };
}

void Fighter::StartFighter2Anim(GameObject& Fighter) {
	Play::SetSprite(Fighter, "FighterRightDestroy_5", 0.2f);
	Fighter.type = MainGame::TYPE_FIGHTER_DESTROYED;
	
	Fighter.velocity = { 0, 0 };
}

void Fighter::DestroyFighter(GameObject& Fighter, GameObject& Bullet) {
	if (Play::IsColliding(Fighter, Bullet)) {
		if (Fighter.spriteId == Play::GetSpriteId("Fighter")) {
			Fighter::StartFighterAnim(Fighter);
		}
		else {
			Fighter::StartFighter2Anim(Fighter);
		}
		Play::DestroyGameObject(Bullet.GetId());
		MainGame::IncrementPoints(1000);
		Play::PlayAudio("EnemyDeath");
	}
}

void Fighter::FireBullet(GameObject& Fighter, bool IsLeftFighter) {
	int NewBulletID = Bullet::InstantiateEnemyBullet(Fighter.pos);
	GameObject& NewBullet = Play::GetGameObject(NewBulletID);
	if (IsLeftFighter) {
		Bullet::MoveBulletRight(NewBullet);
	}
	else {
		Bullet::MoveBulletLeft(NewBullet);
	}
	Play::PlayAudio("EnemyFire");
}

void Fighter::MoveFighter(GameObject& Fighter) {
	Fighter.velocity = -Fighter.velocity;
}