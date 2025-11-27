#include "GameWorld.h" 
#include "Random.h"
#include "Player.h" 
#include "Boidy.h"
#include "BehaviorTree.h" 
#include "EnemyManager.h" 
#include "SceneManager.h"
#include "PostMaster/PostMaster.h"


GameWorld::~GameWorld()
{
}

bool GameWorld::Init()
{
	//if (!mySprite.Init("Sprites/Ball_Throw.dds")) 
	// // return false; 
	// //mySprite.SetPosition({ 650,300 }); 

	std::unique_ptr<Player> player = std::make_unique<Player>();
	Player* playerRaw = player.get();
	gameObjects.push_back((std::move(player)));

	EnemyManager& enemyManager = EnemyManager::GetInstance();


	for (size_t i = 0; i < 20; i++)
	{
		std::unique_ptr<Boidy> boid = std::make_unique<Boidy>();
		Boidy* boidyRaw = boid.get();
		gameObjects.push_back(std::move(boid));
		enemyManager.AddEnemy(boidyRaw);
	}

	if (!enemyManager.AssignFormationSlots())
		return false;

	for (std::unique_ptr<GameObject>& gameObject : gameObjects)
	{
		if (!gameObject->Init())
			return false;
	}

	enemyManager.SetPlayer(playerRaw); // Should make local variable, but not sure how it works with unique_ptr... 

	return true;
}
void GameWorld::Update(float aDeltaTime)
{
	if (GetAsyncKeyState(VK_ESCAPE)) // So you can quit the application
	{
		SceneManager::GetInstance().shouldRun = false;
	}

	EnemyManager& enemyManager = EnemyManager::GetInstance();

	if (GetAsyncKeyState('1'))
	{
		enemyManager.AlertEnemies(true);
		enemyManager.ShouldArmy(false);
	}
	if (GetAsyncKeyState('2'))
	{
		enemyManager.ShouldArmy(false);
		enemyManager.AlertEnemies(false);
	}
	if (GetAsyncKeyState('3'))
	{
		enemyManager.ShouldArmy(true);
	}
	enemyManager.UpdateEnemies(aDeltaTime);
	for (std::unique_ptr<GameObject>& go : gameObjects)
	{
		if (go->isActive)
			go->Update(aDeltaTime);
	}

	for (GameObject*& co : currentObjects)
		if (co->isActive)
			co->Update(aDeltaTime);
}
void GameWorld::Render()
{
	//mySprite.Draw(); 
	for (std::unique_ptr<GameObject>& go : gameObjects)
	{
		if (go->isActive) 
			go->Render();
	}

	for (GameObject*& co : currentObjects)
		if (co->isActive)
			co->Render();
}