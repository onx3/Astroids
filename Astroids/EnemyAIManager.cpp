#include "AstroidsPrivate.h"
#include "EnemyAIManager.h"
#include "GameObject.h"
#include <memory>
#include <cassert>
#include <iostream>
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "RandomMovementComponent.h"
#include "ExplosionComponent.h"
#include "DropManager.h"
#include "ResourceManager.h"


Timer gTimers[3];
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

void EnemyAIManager::Update(float deltaTime)
{
    for (auto * pEnemy : mEnemyObjects)
    {
        if (pEnemy && !pEnemy->IsDestroyed())
        {
            auto pHealthComp = pEnemy->GetComponent<HealthComponent>().lock();
            if (pHealthComp)
            {
                pHealthComp->SetDeathCallBack([this, pEnemy]() {
                    OnDeath(pEnemy);
                    });
            }
        }
    }
	CleanUpDeadEnemies();

	while (mEnemyObjects.size() < mMaxEnemies)
	{
		sf::Vector2f spawnPosition = GetRandomSpawnPosition();
		RespawnEnemy(EEnemy::Asteroid, spawnPosition);
	}
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAIManager::OnGameEnd()
{
    mEnemyObjects.clear();
}

//------------------------------------------------------------------------------------------------------------------------

sf::Vector2f EnemyAIManager::GetRandomSpawnPosition()
{
    const int screenWidth = mpGameManager->mpWindow->getSize().x;
    const int screenHeight = mpGameManager->mpWindow->getSize().y;

    const float spawnOffset = 500.0f; // Increased offset for smoother appearance

    sf::Vector2f spawnPosition;

    int edge = rand() % 4;

    switch (edge)
    {
        case 0: // Top
            spawnPosition.x = static_cast<float>(rand() % screenWidth);
            spawnPosition.y = -spawnOffset;
            break;

        case 1: // Bottom
            spawnPosition.x = static_cast<float>(rand() % screenWidth);
            spawnPosition.y = static_cast<float>(screenHeight) + spawnOffset;
            break;

        case 2: // Left
            spawnPosition.x = -spawnOffset;
            spawnPosition.y = static_cast<float>(rand() % screenHeight);
            break;

        case 3: // Right
            spawnPosition.x = static_cast<float>(screenWidth) + spawnOffset;
            spawnPosition.y = static_cast<float>(rand() % screenHeight);
            break;
    }
    return spawnPosition;
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAIManager::RemoveEnemy(GameObject * pEnemy)
{
    pEnemy->Destroy();
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
        auto * pEnemy = mpGameManager->CreateNewGameObject(ETeam::Enemy, mpGameManager->GetRootGameObject());
        mEnemyObjects.push_back(pEnemy);

        auto pSpriteComp = pEnemy->GetComponent<SpriteComponent>().lock();

        if (pSpriteComp)
        {
            {
                gTimers[0].Start();
                std::string file = GetEnemyFile(type);
                auto scale = sf::Vector2f(.08f, .08f);
                ResourceId resourceId(file);
                auto pTexture = mpGameManager->GetManager<ResourceManager>()->GetTexture(resourceId);

                pSpriteComp->SetSprite(pTexture, scale);
                pSpriteComp->SetPosition(pos);
                pEnemy->AddComponent(pSpriteComp);
                gTimers[0].Stop();
            }
            {
                gTimers[1].Start();

                // Add random movement AFTER smooth intro
                auto pRandomMovementComp = std::make_shared<RandomMovementComponent>(pEnemy);
                pEnemy->AddComponent(pRandomMovementComp);

                auto pHealthComponent = std::make_shared<HealthComponent>(pEnemy, 10, 100, 1, 1);
                pEnemy->AddComponent(pHealthComponent);
                gTimers[1].Stop();
            }
            {
                gTimers[2].Start();
                pEnemy->CreatePhysicsBody(&mpGameManager->GetPhysicsWorld(), pEnemy->GetSize(), true);
                auto pCollisionComp = std::make_shared<CollisionComponent>(
                    pEnemy,
                    &mpGameManager->GetPhysicsWorld(),
                    pEnemy->GetPhysicsBody(),
                    pEnemy->GetSize(),
                    true
                );
                pEnemy->AddComponent(pCollisionComp);
                gTimers[2].Stop();
            }
            
        }
        
    }
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAIManager::DestroyAllEnemies()
{
    for (auto * pEnemy : mEnemyObjects)
    {
        pEnemy->Destroy();
    }
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAIManager::CleanUpDeadEnemies()
{
    for (auto * pEnemy : mEnemyObjects)
    {
        if (pEnemy && !pEnemy->IsDestroyed())
        {
            auto explosionComp = pEnemy->GetComponent<ExplosionComponent>().lock();
            if (explosionComp && explosionComp->IsAnimationFinished())
            {
                pEnemy->Destroy();
            }
        }
    }	

    auto removeStart = std::remove_if(mEnemyObjects.begin(), mEnemyObjects.end(),
        [](GameObject * pObj)
        {
            return pObj->IsDestroyed();
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

void EnemyAIManager::OnDeath(GameObject * pEnemy)
{
    // Explosion
    if (!pEnemy->GetComponent<ExplosionComponent>().lock())
    {
        auto explosionComp = std::make_shared<ExplosionComponent>(
            pEnemy, "Art/explosion.png", 32, 32, 7, 0.1f, sf::Vector2f(2.f, 2.f), pEnemy->GetPosition());
        pEnemy->AddComponent(explosionComp);
    }
    // Add Score
    {
        auto pScoreManager = mpGameManager->GetManager<ScoreManager>();
        pScoreManager->AddScore(1000);
    }
    // Drops
    {
        EDropType dropType = DetermineDropType();
        auto pDropManager = mpGameManager->GetManager<DropManager>();
        if (pDropManager)
        {
            sf::Vector2f position = pEnemy->GetPosition();
            pDropManager->SpawnDrop(dropType, position);
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------

EDropType EnemyAIManager::DetermineDropType() const
{
    int randomValue = rand() % 100;

    if (randomValue < 3)
    {
        return EDropType::NukePickup;
    }
    else if (randomValue < 7)
    {
        return EDropType::LifePickup;
    }

    return EDropType::None;
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------