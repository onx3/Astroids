#include "PlayerManager.h"
#include "SpriteComponent.h"
#include "ControlledMovementComponent.h"
#include "ProjectileComponent.h"
#include "HealthComponent.h"
#include "CollisionComponent.h"

PlayerManager::PlayerManager(GameManager * pGameManager)
	: BaseManager(pGameManager)
{
    InitPlayer();
}

//------------------------------------------------------------------------------------------------------------------------

PlayerManager::~PlayerManager()
{
}

//------------------------------------------------------------------------------------------------------------------------

void PlayerManager::InitPlayer()
{
    auto * pPlayer = mpGameManager->CreateNewGameObject(ETeam::Friendly, mpGameManager->GetRootGameObject());

    mPlayerObjects.push_back(pPlayer);

    // Sprite Component
    {
        sf::Vector2u windowSize = mpGameManager->mpWindow->getSize();
        sf::Vector2f centerPosition(float(windowSize.x) / 2.0f, float(windowSize.y) / 2.0f);

        auto pSpriteComponent = pPlayer->GetComponent<SpriteComponent>().lock();

        if (pSpriteComponent)
        {
            std::string file = "Art/player.png";
            auto scale = sf::Vector2f(.5f, .5f);
            pSpriteComponent->SetSprite(file, scale);
            pSpriteComponent->SetPosition(centerPosition);
        }
    }

    // Controlled Movement Component
    {
        auto pMovementComponent = pPlayer->GetComponent<ControlledMovementComponent>().lock();
        if (!pMovementComponent)
        {
            auto pMovementComponent = std::make_shared<ControlledMovementComponent>(pPlayer);
            pPlayer->AddComponent(pMovementComponent);
        }
    }

    // Projectile Component
    {
        auto pProjectileComponent = pPlayer->GetComponent<ProjectileComponent>().lock();
        if (!pProjectileComponent)
        {
            pPlayer->AddComponent(std::make_shared<ProjectileComponent>(pPlayer));
        }
    }

    // Health Component
    {
        auto pHealthComponent = pPlayer->GetComponent<HealthComponent>().lock();
        if (!pHealthComponent)
        {
            pPlayer->AddComponent(std::make_shared<HealthComponent>(pPlayer, 100, 100, 3, 3, 2.f));
        }
    }

    // Collision Component
    {
        auto pCollisionComponent = pPlayer->GetComponent<CollisionComponent>().lock();
        if (!pCollisionComponent)
        {
            pPlayer->AddComponent(std::make_shared<CollisionComponent>(pPlayer, pPlayer->GetSize()));
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------

void PlayerManager::Update()
{
    CleanUpDeadPlayers();

    if (mPlayerObjects.empty())
    {
        // Call GameManager to end the game
        mpGameManager->EndGame();
    }
    else
    {
        for (auto * pPlayer : mPlayerObjects)
        {
            auto pHealthComp = pPlayer->GetComponent<HealthComponent>().lock();
            if (pHealthComp->GetLives() == 1)
            {
                auto pSpriteComponent = pPlayer->GetComponent<SpriteComponent>().lock();
                pSpriteComponent->SetSprite("Art/playerDamaged.png", pSpriteComponent->GetSprite().getScale());
            }
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------

void PlayerManager::CleanUpDeadPlayers()
{
    auto newEnd = std::remove_if(mPlayerObjects.begin(), mPlayerObjects.end(),
        [](GameObject * obj)
        {
            return obj->IsDestroyed();
        });

    mPlayerObjects.erase(newEnd, mPlayerObjects.end());
}


//------------------------------------------------------------------------------------------------------------------------

const std::vector<GameObject *> & PlayerManager::GetPlayers() const
{
	return mPlayerObjects;
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------