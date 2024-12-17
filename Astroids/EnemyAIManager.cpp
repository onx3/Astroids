#include "EnemyAIManager.h"
#include "GameObject.h"
#include <memory>
#include <cassert>
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "RandomMovementComponent.h"

EnemyAIManager::EnemyAIManager(GameManager * pGameManager)
	: mpGameManager(pGameManager)
{

}

//------------------------------------------------------------------------------------------------------------------------

EnemyAIManager::EnemyAIManager(GameManager * pGameManager, int enemyCount)
	: mpGameManager(pGameManager)
{
	AddEnemies(enemyCount, EEnemy::Asteroid, sf::Vector2f());
}

//------------------------------------------------------------------------------------------------------------------------

EnemyAIManager::~EnemyAIManager()
{

}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAIManager::Update()
{
	CleanUpDeadEnemies();
	while (mEnemyObjects.size() < mMaxEnemies)
	{
		sf::Vector2f spawnPosition = GetRandomSpawnPosition();
		RespawnEnemy(EEnemy::Asteroid, spawnPosition);
	}
}

//------------------------------------------------------------------------------------------------------------------------

sf::Vector2f EnemyAIManager::GetRandomSpawnPosition()
{
	const int screenWidth = mpGameManager->mpWindow->getSize().x;
	const int screenHeight = mpGameManager->mpWindow->getSize().y;

	const sf::FloatRect noSpawnZone(
		screenWidth / 4.f,         // X (left boundary of the box)
		screenHeight / 4.f,        // Y (top boundary of the box)
		screenWidth / 2.f,         // Width (center box)
		screenHeight / 2.f         // Height (center box)
	);

	sf::Vector2f spawnPosition;

	// Keep generating a random position until it's outside the no-spawn zone
	do
	{
		spawnPosition.x = static_cast<float>(rand() % screenWidth);
		spawnPosition.y = static_cast<float>(rand() % screenHeight);
	}
	while (noSpawnZone.contains(spawnPosition));

	return spawnPosition;
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAIManager::RemoveEnemy(GameObject * enemy)
{
	enemy->Destroy(); // Use GameObject's Destroy function
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAIManager::RespawnEnemy(EEnemy type, sf::Vector2f pos)
{
	AddEnemies(1, type, pos);
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAIManager::AddEnemies(int count, EEnemy type, sf::Vector2f pos)
{
    for (int i = 0; i < count; ++i)
    {
        GameObject * pGameObj = new GameObject(mpGameManager, ETeam::Enemy);
        auto pSpriteComp = pGameObj->GetComponent<SpriteComponent>().lock();

        if (pSpriteComp)
        {
            std::string file = GetEnemyFile(type);
			auto scale = sf::Vector2f(.08, .08f);
            pSpriteComp->SetSprite(file, scale);
			pSpriteComp->SetPosition(pos);
            pGameObj->AddComponent(pSpriteComp);

            auto pRandomMovementComp = std::make_shared<RandomMovementComponent>(pGameObj);
            pGameObj->AddComponent(pRandomMovementComp);

            auto pHealthComponent = std::make_shared<HealthComponent>(pGameObj, 10, 100, 1, 1);
            pGameObj->AddComponent(pHealthComponent);

            auto pCollisionComp = std::make_shared<CollisionComponent>(pGameObj, pGameObj->GetSize());
            pGameObj->AddComponent(pCollisionComp);
        }

        mpGameManager->GetGameObjects().push_back(pGameObj);
		mEnemyObjects.push_back(pGameObj);
    }
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAIManager::CleanUpDeadEnemies()
{
	// Clean up global GameObjects
	auto & gameObjects = mpGameManager->GetGameObjects();
	gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(),
		[](GameObject * obj) { return obj->IsDestroyed(); }),
		gameObjects.end());

	// Clean up local enemy objects
	mEnemyObjects.erase(std::remove_if(mEnemyObjects.begin(), mEnemyObjects.end(),
		[](GameObject * obj) { return obj->IsDestroyed(); }),
		mEnemyObjects.end());
}

//------------------------------------------------------------------------------------------------------------------------

std::string EnemyAIManager::GetEnemyFile(EEnemy type)
{
	switch (type)
	{
		case (EEnemy::Ship):
		{
			return "Art/EnemyShip.png";
		}
		case (EEnemy::Ufo):
		{
			return "Art/EnemyUFO.png";
		}
		case (EEnemy::Asteroid):
		{
			return "Art/Astroid.png";
		}
		default:
		{
			return "Art/Astroid.png";
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------

const std::vector<GameObject *> & EnemyAIManager::GetEnemies() const
{
	return mEnemyObjects;
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------