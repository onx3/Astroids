#include "DropManager.h"
#include "GameManager.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "ResourceManager.h"

DropManager::DropManager(GameManager * pGameManager)
	: BaseManager(pGameManager)
{

}

//------------------------------------------------------------------------------------------------------------------------

void DropManager::Update()
{
    CleanUpDrops();
}

//------------------------------------------------------------------------------------------------------------------------

void DropManager::OnGameEnd()
{
    mDropObjects.clear();
}

//------------------------------------------------------------------------------------------------------------------------

void DropManager::CleanUpDrops()
{
    auto removeStart = std::remove_if(mDropObjects.begin(), mDropObjects.end(),
        [](GameObject * pObj)
        {
            return pObj->IsDestroyed();
        });

    mDropObjects.erase(removeStart, mDropObjects.end());
}

//------------------------------------------------------------------------------------------------------------------------

void DropManager::SpawnDrop(EDropType dropType, const sf::Vector2f & position)
{
    if (dropType == EDropType::None) return;

    GameObject * pDrop = nullptr;
    if (dropType == EDropType::NukePickup)
    {
        pDrop = mpGameManager->CreateNewGameObject(ETeam::NukeDrop, mpGameManager->GetRootGameObject());
    }
    else if (dropType == EDropType::LifePickup)
    {
        pDrop = mpGameManager->CreateNewGameObject(ETeam::LifeDrop, mpGameManager->GetRootGameObject());
    }
    if (pDrop != nullptr)
    {
        mDropObjects.push_back(pDrop);
        auto pSpriteComp = pDrop->GetComponent<SpriteComponent>().lock();

        if (pSpriteComp)
        {
            std::shared_ptr<sf::Texture> pSpriteTexture;
            std::string file;
            ResourceId resourceId("");

            switch (dropType)
            {
                case EDropType::NukePickup:
                    file = "Art/Nuke.png";
                    resourceId = ResourceId(file);
                    pSpriteTexture = mpGameManager->GetManager<ResourceManager>()->GetTexture(resourceId);
                    if (pSpriteTexture)
                    {
                        pSpriteComp->SetSprite(pSpriteTexture, sf::Vector2f(.75, .75));
                    }
                    break;
                case EDropType::LifePickup:
                    file = "Art/Life.png";
                    resourceId = ResourceId(file);
                    pSpriteTexture = mpGameManager->GetManager<ResourceManager>()->GetTexture(resourceId);
                    if (pSpriteTexture)
                    {
                        pSpriteComp->SetSprite(pSpriteTexture, sf::Vector2f(.75, .75));
                    }
                    break;
                default:
                    return;
            }

            pSpriteComp->SetPosition(position);
            pDrop->AddComponent(pSpriteComp);

            // Add collision or interaction logic for pickup
            pDrop->CreatePhysicsBody(&mpGameManager->GetPhysicsWorld(), pDrop->GetSize(), true);

            auto pCollisionComp = std::make_shared<CollisionComponent>(
                pDrop, &mpGameManager->GetPhysicsWorld(), pDrop->GetPhysicsBody(), pDrop->GetSize(), true);
            pDrop->AddComponent(pCollisionComp);
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------