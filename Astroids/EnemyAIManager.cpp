#include "EnemyAIManager.h"
#include "GameObject.h"
#include <memory>
#include <cassert>
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "RandomMovementComponent.h"

EnemyAIManager::EnemyAIManager(GameManager * pGameManager)
	: BaseManager(pGameManager)
{

}

//------------------------------------------------------------------------------------------------------------------------

EnemyAIManager::EnemyAIManager(GameManager * pGameManager, int enemyCount)
	: BaseManager(pGameManager)
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

    sf::Vector2f spawnPosition;

    // Randomly decide which edge the enemy spawns outside of
    int edge = rand() % 4; // 0 = Top, 1 = Bottom, 2 = Left, 3 = Right

    switch (edge)
    {
        case 0: // Top (above screen)
            spawnPosition.x = static_cast<float>(rand() % screenWidth);
            spawnPosition.y = -50.0f; // Slightly above the screen
            break;

        case 1: // Bottom (below screen)
            spawnPosition.x = static_cast<float>(rand() % screenWidth);
            spawnPosition.y = static_cast<float>(screenHeight) + 50.0f; // Slightly below the screen
            break;

        case 2: // Left (left of screen)
            spawnPosition.x = -50.0f; // Slightly left of the screen
            spawnPosition.y = static_cast<float>(rand() % screenHeight);
            break;

        case 3: // Right (right of screen)
            spawnPosition.x = static_cast<float>(screenWidth) + 50.0f; // Slightly right of the screen
            spawnPosition.y = static_cast<float>(rand() % screenHeight);
            break;
    }

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
			auto scale = sf::Vector2f(.08f, .08f);
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
	auto removeStart = std::remove_if(mEnemyObjects.begin(), mEnemyObjects.end(),
		[](GameObject * obj)
		{
			return obj->IsDestroyed();
		});

	mEnemyObjects.erase(removeStart, mEnemyObjects.end());
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