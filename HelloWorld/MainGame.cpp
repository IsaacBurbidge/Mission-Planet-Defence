#define PLAY_IMPLEMENTATION
#include <string>
#include <time.h>
#include "Bullet.h"
#include "MainGame.h"
#include "Enemy.h"
#include "Dreadnaught.h"
#include "Fighter.h"

bool DEBUG_INVINSIBLE = true;
int DISPLAY_WIDTH = 496;
int DISPLAY_HEIGHT = 256;
int DISPLAY_SCALE = 2;
bool PLAYING = false;
bool ALIVE = true;
int Lives = 3;
int Points = 0;
int WaveNumber = 1;
int NumberOfEnemies = 5;
bool SpawningWave = false;
int DelayBombDrop = 0;
int DelayEnemyFire = 0;
bool StartCombatMusicPlaying = false;
int InvulnerabilityFrames = 0;
bool Invulnerable = false;

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

void MainGame::IncrementPoints(int PointValue) {
	Points += PointValue;
}

void MainGame::SetAlive() {
	if (Lives == 0) {
		ALIVE = false;
		Play::StopAudioLoop("FightMusic");
		Play::StartAudioLoop("MenuMusic");
	}
}

void MainGame::DecreaseLives() {
	if (!Invulnerable) {
		if (!DEBUG_INVINSIBLE) {
			Lives--;
		}
		InvulnerabilityFrames = 100;
		Invulnerable = true;
		GameObject& Player = Play::GetGameObjectByType(TYPE_PLAYER);
		Play::SetSprite(Player, "PlayerInvulnerable_4", 0.05f);
	}
}

Point2D RandomPosGenerator(Point2D RandomNumber, int i) {
	srand(i+ time(NULL));
	int Seed = rand();
	srand(Seed);
	RandomNumber.x = (rand()% 470)+1;
	RandomNumber.y = (rand()% 240)+1;
	return RandomNumber;
}

void CreateObject(int TYPE,int WIDTH,int HEIGHT,int Collision, const char* SpriteName) {
	Play::CreateGameObject(TYPE, { WIDTH,HEIGHT }, Collision, SpriteName);
	Play::CentreSpriteOrigin(SpriteName);
}

void MainMenu() {
	GameObject& StartButton = Play::GetGameObjectByType(TYPE_START_BUTTON);
	Play::DrawObject(StartButton);
	GameObject& ExitButton = Play::GetGameObjectByType(TYPE_EXIT_BUTTON);
	Play::DrawObject(ExitButton);
	if (Play::KeyPressed(VK_LBUTTON)) {
		Point2D MousePos = Play::GetMousePos();
		if (StartButton.pos.x + 34 > MousePos.x && StartButton.pos.y + 12 > MousePos.y && StartButton.pos.x - 34 < MousePos.x && StartButton.pos.y - 12 < MousePos.y) {
			PLAYING = true;
			StartCombatMusicPlaying = true;
			Play::PlayAudio("Select");
		}
		else if (ExitButton.pos.x + 34 > MousePos.x && ExitButton.pos.y + 12 > MousePos.y && ExitButton.pos.x - 34 < MousePos.x && ExitButton.pos.y - 12 < MousePos.y) {
			Play::PlayAudio("Select");
			MainGameExit();
		}
	}
}

void ResetGame() {
	std::vector<int> PlayerBulletID = Play::CollectGameObjectIDsByType(TYPE_BULLET);
	for (int i = 0; i < PlayerBulletID.size(); i++) {
		Play::DestroyGameObject(PlayerBulletID[i]);
	}
	std::vector<int> EnemyID = Play::CollectGameObjectIDsByType(TYPE_ENEMY);
	for (int i = 0; i < EnemyID.size(); i++) {
		Play::DestroyGameObject(EnemyID[i]);
	}
	std::vector<int> BombID = Play::CollectGameObjectIDsByType(TYPE_BOMB);
	for (int i = 0; i < BombID.size(); i++) {
		Play::DestroyGameObject(BombID[i]);
	}
	std::vector<int> DreadnaughtID = Play::CollectGameObjectIDsByType(TYPE_DREADNAUGHT);
	for (int i = 0; i < DreadnaughtID.size(); i++) {
		Play::DestroyGameObject(DreadnaughtID[i]);
	}
	std::vector<int> FighterID = Play::CollectGameObjectIDsByType(TYPE_FIGHTER);
	for (int i = 0; i < FighterID.size(); i++) {
		Play::DestroyGameObject(FighterID[i]);
	}
	WaveNumber = 1;
	NumberOfEnemies = 5;
	Lives = 3;
	GameObject& Player = Play::GetGameObjectByType(TYPE_PLAYER);
	Player.pos = { DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2 };
}

void EndGame() {
	CreateObject(TYPE_MAINMENU_BUTTON, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2 + 60,10,"MainMenuButton");
	GameObject& MainMenuButton = Play::GetGameObjectByType(TYPE_MAINMENU_BUTTON);
	Play::DrawObject(MainMenuButton);
	std::string Text = "Score: " + std::to_string(Points);
	const char* CharText = Text.c_str();
	Play::DrawDebugText({ DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2 }, CharText);
	if (Play::KeyPressed(VK_LBUTTON)) {
		Point2D MousePos = Play::GetMousePos();
		if (MainMenuButton.pos.x + 34 > MousePos.x && MainMenuButton.pos.y + 12 > MousePos.y && MainMenuButton.pos.x - 34 < MousePos.x && MainMenuButton.pos.y - 12 < MousePos.y) {
			PLAYING = false;
			ALIVE = true;
			Points = 0;
			ResetGame();
			Play::PlayAudio("Select");
		}
	}
}

void DEBUG_SET_INVINSIBLE() {
	if (Play::KeyDown(VK_SHIFT)) {
		DEBUG_INVINSIBLE = !DEBUG_INVINSIBLE;
	}
}

void PlayerMovement(GameObject& Player) {
	Player.acceleration = { 0,0 };
	if (Play::KeyDown(VK_UP)){
		Player.acceleration += { 0, -0.2f };
	}
	else if (Play::KeyDown(VK_DOWN)){
		Player.acceleration += { 0, 0.2f };
	}
	if (Play::KeyDown(VK_LEFT)){
		Player.acceleration += { -0.2f, 0 };
	}
	else if (Play::KeyDown(VK_RIGHT)){
		Player.acceleration += { 0.2f, 0 };
	}
	Play::UpdateGameObject(Player);
	if (Play::IsLeavingDisplayArea(Player)) {
		Player.pos = Player.oldPos;
	}
	Player.acceleration = { 0,0 };
	Player.velocity *= 0.93f;
}

void FireDirection(GameObject& PlayerBullet) {
	if (Play::KeyDown(0x57)){//W - fires up
		PlayerBullet.velocity = {0,-2};
		PlayerBullet.rotation = 90;
	}else if (Play::KeyDown(0x53)) {//S - fires down
		PlayerBullet.velocity = { 0,2 };
		PlayerBullet.rotation = -90;
	}else if (Play::KeyDown(0x41)) {//A - fires left
		PlayerBullet.velocity = { -2,0 };
		PlayerBullet.rotation = 0;
	}else if (Play::KeyDown(0x44)) {//D - fires right
		PlayerBullet.velocity = { 2,0 };
		PlayerBullet.rotation = 180;
	}else {
		PlayerBullet.velocity = { -2,0 };
	}
}

void PlayerFire(GameObject& Player) {
	std::vector<int> PlayerBulletID = Play::CollectGameObjectIDsByType(TYPE_BULLET);
	for (int i = 0; i < PlayerBulletID.size(); i++) {
		GameObject& PlayerBullet = Play::GetGameObject(PlayerBulletID[i]);
		Play::UpdateGameObject(PlayerBullet);
		Play::DrawObjectRotated(PlayerBullet);
	}
	if (Play::KeyPressed(0x57)|| Play::KeyPressed(0x53)|| Play::KeyPressed(0x41)|| Play::KeyPressed(0x44)) {
		Bullet::InstantiatePlayerBullet(Player.pos);
		PlayerBulletID = Play::CollectGameObjectIDsByType(TYPE_BULLET);
		GameObject& PlayerBullet = Play::GetGameObject(PlayerBulletID[PlayerBulletID.size()-1]);
		FireDirection(PlayerBullet);
		Play::PlayAudio("PlayerFire");
	}
}

void Invulnerabilty() {
	if (InvulnerabilityFrames > 0) {
		InvulnerabilityFrames--;
	}
	else {
		Invulnerable = false;
		GameObject& Player = Play::GetGameObjectByType(TYPE_PLAYER);
		Play::SetSprite(Player, "Player", 0);
	}
}

void PlayerControls(GameObject& Player) {
	Play::DrawObject(Player);
	PlayerMovement(Player);
	PlayerFire(Player);
	Invulnerabilty();
	DEBUG_SET_INVINSIBLE();
}

void EnemyCreation(Point2D Pos) {
	Enemy::InstantiateEnemy(Pos);
}

void DreadnaughtCreation(Point2D Pos) {
	Dreadnaught::InstantiateDreadnaught(Pos);
	GameObject& Dreadnaught = Play::GetGameObjectByType(TYPE_DREADNAUGHT);
	Dreadnaught::MoveDreadnaught(Dreadnaught);
}

void FighterCreation(Point2D Pos) {
	Fighter::InstantiateFighter(Pos);
}

void Fighter2Creation(Point2D Pos) {
	Fighter::InstantiateFighter2(Pos);
}

void IncrementWave() {
	WaveNumber += 1;
	NumberOfEnemies += 1;
	SpawningWave = false;
}

void EnemyDestruction() {
	std::vector<int> EnemyID = Play::CollectGameObjectIDsByType(TYPE_ENEMY);
	std::vector<int> PlayerBulletID = Play::CollectGameObjectIDsByType(TYPE_BULLET);
	for (int i = 0; i < EnemyID.size(); i++) {
		for (int j = 0; j < PlayerBulletID.size(); j++) {
			GameObject& Enemy = Play::GetGameObject(EnemyID[i]);
			GameObject& PlayerBullet = Play::GetGameObject(PlayerBulletID[j]);
			Enemy::DestroyEnemy(Enemy,PlayerBullet);
		}
	}
}

void DreadnaughtDestruction() {
	GameObject& Dreadnaught = Play::GetGameObjectByType(TYPE_DREADNAUGHT);
	std::vector<int> PlayerBulletID = Play::CollectGameObjectIDsByType(TYPE_BULLET);
	for (int i = 0; i < PlayerBulletID.size(); i++) {
		GameObject& PlayerBullet = Play::GetGameObject(PlayerBulletID[i]);
		Dreadnaught::DestroyDreadnaught(Dreadnaught, PlayerBullet);
	}
	if (Dreadnaught.frame == 5) {
		Play::DestroyGameObject(Dreadnaught.GetId());
	}
}

void UpdateDestroyedFighter() {
	std::vector<int> FighterID = Play::CollectGameObjectIDsByType(TYPE_FIGHTER_DESTROYED);
	for (int i = 0; i < FighterID.size(); i++) {
		GameObject& Fighter = Play::GetGameObject(FighterID[i]);
		Play::UpdateGameObject(Fighter);
		Play::DrawObject(Fighter);
		if (Fighter.frame == 5) {
			Play::DestroyGameObject(Fighter.GetId());
		}
	}
}

void FighterDestruction() {
	std::vector<int> FighterID = Play::CollectGameObjectIDsByType(TYPE_FIGHTER);
	std::vector<int> PlayerBulletID = Play::CollectGameObjectIDsByType(TYPE_BULLET);
	for (int i = 0; i < FighterID.size(); i++) {
		for (int j = 0; j < PlayerBulletID.size(); j++) {
			GameObject& Fighter = Play::GetGameObject(FighterID[i]);
			GameObject& PlayerBullet = Play::GetGameObject(PlayerBulletID[j]);
			Fighter::DestroyFighter(Fighter, PlayerBullet);
		}
	}
	UpdateDestroyedFighter();
}

void UpdateEnemy(GameObject& Player) {
	std::vector<int> EnemyID = Play::CollectGameObjectIDsByType(TYPE_ENEMY);
	for (int i = 0; i < EnemyID.size(); i++) {
		GameObject& Enemy = Play::GetGameObject(EnemyID[i]);
		Enemy::MoveTowardsPlayer(Enemy, Player);
		Play::UpdateGameObject(Enemy);
		if (Play::IsLeavingDisplayArea(Enemy)) {
			Enemy.pos = Enemy.oldPos;
		}
		Play::DrawObject(Enemy);
		Enemy::KillPlayer(Enemy, Player);
		if (Enemy.frame == 5) {
			Play::DestroyGameObject(Enemy.GetId());
		}
	}
}

void UpdateBomb(GameObject& Player) {
	std::vector<int> BombID = Play::CollectGameObjectIDsByType(TYPE_BOMB);
	for (int i = 0; i < BombID.size(); i++) {
		GameObject& Bomb = Play::GetGameObject(BombID[i]);
		Play::UpdateGameObject(Bomb);
		Play::DrawObject(Bomb);
		if (Play::IsLeavingDisplayArea(Bomb)) {
			Play::DestroyGameObject(Bomb.GetId());
		}
		else {
			Bomb::KillPlayer(Bomb, Player);
		}
		if (Bomb.frame == 8) {
			Play::DestroyGameObject(Bomb.GetId());
		}
	}
}

void UpdateDreadnaught(GameObject& Player) {
	GameObject& Dreadnaught = Play::GetGameObjectByType(TYPE_DREADNAUGHT);
	Play::UpdateGameObject(Dreadnaught);
	if (Play::IsLeavingDisplayArea(Dreadnaught)) {
		Dreadnaught.pos = Dreadnaught.oldPos;
		Dreadnaught::ChangeDreadnaughtDirection(Dreadnaught);
	}
	if (Play::IsVisible(Dreadnaught)) {
		DelayBombDrop++;
		if (DelayBombDrop % 50 == 0) {
			Dreadnaught::DropBomb(Dreadnaught);
		}
		UpdateBomb(Player);
	}
	Play::DrawObject(Dreadnaught);
}

void UpdateEnemyBullet(GameObject& Player) {
	std::vector<int> BulletID = Play::CollectGameObjectIDsByType(TYPE_ENEMYBULLET);
	for (int i = 0; i < BulletID.size(); i++) {
		GameObject& Bullet = Play::GetGameObject(BulletID[i]);
		Play::UpdateGameObject(Bullet);
		Play::DrawObject(Bullet);
		if (Play::IsLeavingDisplayArea(Bullet)) {
			Play::DestroyGameObject(Bullet.GetId());
		}
		else {
			Bullet::KillPlayer(Bullet, Player);
		}
	}
}

void UpdateFighter(GameObject& Player) {
	std::vector<int> FighterID = Play::CollectGameObjectIDsByType(TYPE_FIGHTER);
	for (int i = 0; i < FighterID.size(); i++) {
		GameObject& Fighter = Play::GetGameObject(FighterID[i]);
		Play::UpdateGameObject(Fighter);
		if (Play::IsLeavingDisplayArea(Fighter)) {
			Fighter.pos = Fighter.oldPos;
			Fighter::MoveFighter(Fighter);
		}
		Play::DrawObject(Fighter);
	}
	if (FighterID.size() == 1) {
		GameObject& Fighter = Play::GetGameObject(FighterID[0]);
		
		DelayEnemyFire++;
		if (DelayEnemyFire % 30 == 0) {
			int SpriteID = Play::GetSpriteId("Fighter");
			if (Fighter.spriteId == SpriteID) {
				Fighter::FireBullet(Fighter, true);
			}else{
				Fighter::FireBullet(Fighter, false);
			}
		}
		UpdateEnemyBullet(Player);
	}
	else if (FighterID.size() == 2) {
		GameObject& Fighter = Play::GetGameObject(FighterID[0]);
		GameObject& Fighter2 = Play::GetGameObject(FighterID[1]);
		DelayEnemyFire++;
		if (DelayEnemyFire % 30 == 0) {
			Fighter::FireBullet(Fighter,true);
			Fighter::FireBullet(Fighter2,false);
		}
		UpdateEnemyBullet(Player);
	}
}

void CheckForNewWave() {
	std::vector<int> EnemyID = Play::CollectGameObjectIDsByType(TYPE_ENEMY);
	std::vector<int> DreadnaughtID = Play::CollectGameObjectIDsByType(TYPE_DREADNAUGHT);
	std::vector<int> FighterID = Play::CollectGameObjectIDsByType(TYPE_FIGHTER);
	if (EnemyID.size() == 0 && DreadnaughtID.size() == 0 && FighterID.size() == 0) {
		IncrementWave();
	}
}

void StartWave() {
	if (!SpawningWave) {
		for (int i = 0; i < NumberOfEnemies; i++) {
			Point2D Pos;
			Pos = RandomPosGenerator(Pos, i);
			EnemyCreation(Pos);
		}
		if (WaveNumber > 4) {
			Point2D Pos = { (DISPLAY_WIDTH / 2) ,(DISPLAY_HEIGHT / 10) };
			DreadnaughtCreation(Pos);
		}
		if (WaveNumber > 9) {
			Point2D Pos = { (DISPLAY_WIDTH / 10) ,(DISPLAY_HEIGHT / 2) };
			FighterCreation(Pos);
			Pos = { (DISPLAY_WIDTH - 50) ,(DISPLAY_HEIGHT/2) };
			Fighter2Creation(Pos);
			std::vector<int> FighterID = Play::CollectGameObjectIDsByType(TYPE_FIGHTER);
			GameObject& Fighter = Play::GetGameObject(FighterID[0]);
			Fighter.velocity = { 0,1 };
			GameObject& Fighter2 = Play::GetGameObject(FighterID[1]);
			Fighter2.velocity = { 0,-1 };
		}
		SpawningWave = true;
	}
}

void UpdateAllEnemies(GameObject& Player) {
	UpdateEnemy(Player);
	UpdateDreadnaught(Player);
	UpdateFighter(Player);
}

void DestroyAllEnemies() {
	EnemyDestruction();
	DreadnaughtDestruction();
	FighterDestruction();
}

void ManageEnemies(GameObject& Player) {
	StartWave();
	UpdateAllEnemies(Player);
	DestroyAllEnemies();
	CheckForNewWave();
}

void WriteText() {
	std::string Text = "Score: " + std::to_string(Points);
	const char* CharText = Text.c_str();
	Play::DrawDebugText({ DISPLAY_WIDTH / 12, DISPLAY_HEIGHT / 14 }, CharText, Play::cBlue);
	Text = "Wave: " + std::to_string(WaveNumber);
	CharText = Text.c_str();
	Play::DrawDebugText({ DISPLAY_WIDTH - 100, DISPLAY_HEIGHT / 14 }, CharText, Play::cBlue);
	Text = "Lives: " + std::to_string(Lives);
	CharText = Text.c_str();
	Play::DrawDebugText({ DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 14 }, CharText, Play::cBlue);
}

void MainGame() {
	GameObject& Player = Play::GetGameObjectByType(TYPE_PLAYER);
	PlayerControls(Player);
	ManageEnemies(Player);
	WriteText();
}

// The entry point for a PlayBuffer program
void MainGameEntry( PLAY_IGNORE_COMMAND_LINE ){
	Play::CreateManager( DISPLAY_WIDTH, DISPLAY_HEIGHT, DISPLAY_SCALE );
	const char* Background = "Data\\Sprites\\Background.png";
	CreateObject(TYPE_START_BUTTON,DISPLAY_WIDTH/2,DISPLAY_HEIGHT/2, 5,"StartButton");
	CreateObject(TYPE_EXIT_BUTTON, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2+30, 5, "ExitButton");
	CreateObject(TYPE_PLAYER, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT / 2, 5, "Player");
	Play::LoadBackground(Background);
	Play::CentreAllSpriteOrigins();
	Play::StartAudioLoop("MenuMusic");
}

// Called by PlayBuffer every frame (60 times a second!)
bool MainGameUpdate( float elapsedTime ){
	Play::ClearDrawingBuffer( Play::cGrey );
	Play::DrawBackground();
	if (PLAYING == false) {
		MainMenu();
	}
	else if (PLAYING == true) {
		if (ALIVE == true) {
			MainGame();
		}
		else if (ALIVE == false) {
			EndGame();
		}
	}
	if (StartCombatMusicPlaying) {
		StartCombatMusicPlaying = false;
		Play::StopAudioLoop("MenuMusic");
		Play::StartAudioLoop("FightMusic");
	}
	Play::PresentDrawingBuffer();
	return Play::KeyDown( VK_ESCAPE );
}

// Gets called once when the player quits the game 
int MainGameExit( void ){
	Play::DestroyManager();
	return PLAY_OK;
}
