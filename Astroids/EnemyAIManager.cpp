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

void EnemyAIManager::UpdateEnemies()
{
	CleanUpDeadEnemies();
	float rand = 100.f;
	while (mpGameManager->GetGameObjects().size() < mMaxEnemies)
	{
		RespawnEnemy(EEnemy::Asteroid, sf::Vector2f(100.f + rand, 50.f + rand));
		rand += 50;
	}
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
			auto scale = sf::Vector2f(.5, .5f);
            pSpriteComp->SetSprite(file, scale);
			pSpriteComp->SetPosition(pos);
            pGameObj->AddComponent(pSpriteComp);

            auto pRandomMovementComp = std::make_shared<RandomMovementComponent>(pGameObj);
            pGameObj->AddComponent(pRandomMovementComp);

            auto pHealthComponent = std::make_shared<HealthComponent>(pGameObj, 10, 100);
            pGameObj->AddComponent(pHealthComponent);

            auto pCollisionComp = std::make_shared<CollisionComponent>(pGameObj, pGameObj->GetSize());
            pGameObj->AddComponent(pCollisionComp);
        }

        mpGameManager->GetGameObjects().push_back(pGameObj);
    }
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAIManager::CleanUpDeadEnemies()
{
    auto & gameObjects = mpGameManager->GetGameObjects();
    gameObjects.erase(std::remove_if(gameObjects.begin(), gameObjects.end(),
        [](GameObject * obj) { return obj->IsDestroyed(); }),
        gameObjects.end());
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
			return "Art/meteorBig.png";
		}
		default:
		{
			return "Art/meteorBig.png";
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------