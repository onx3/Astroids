#include "AstroidsPrivate.h"
#include "CollisionListener.h"
#include "GameObject.h"
#include "ProjectileComponent.h"
#include "HealthComponent.h"

CollisionListener::CollisionListener(GameManager * pGameManager)
    : mpGameManager(pGameManager)
{
}

//------------------------------------------------------------------------------------------------------------------------

void CollisionListener::BeginContact(b2Contact * contact)
{
    GameObject * pObjectA = reinterpret_cast<GameObject *>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    GameObject * pObjectB = reinterpret_cast<GameObject *>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    if (pObjectA && pObjectB)
    {
        HandleCollision(pObjectA, pObjectB);
    }
}

//------------------------------------------------------------------------------------------------------------------------

void CollisionListener::HandleCollision(GameObject * pObjA, GameObject * pObjB)
{
    // Player hit enemy
    if (pObjA->GetTeam() == ETeam::Player && pObjB->GetTeam() == ETeam::Enemy)
    {
        auto pHealthComp = pObjA->GetComponent<HealthComponent>().lock();
        if (pHealthComp)
        {
            if (pObjB->IsActive())
            {
                pHealthComp->LoseHealth(100);
            }
        }
    }
    else if (pObjA->GetTeam() == ETeam::Enemy && pObjB->GetTeam() == ETeam::Player)
    {
        auto pHealthComp = pObjB->GetComponent<HealthComponent>().lock();
        if (pHealthComp)
        {
            if (pObjB->IsActive())
            {
                pHealthComp->LoseHealth(100);
            }
        }
    }
    // Projectile hit enemy
    else if (pObjA->GetTeam() == ETeam::Friendly && pObjB->GetTeam() == ETeam::Enemy)
    {
        if (pObjB->IsActive())
        {
            auto pObjBHealthComp = pObjB->GetComponent<HealthComponent>().lock();
            if (pObjBHealthComp)
            {
                pObjBHealthComp->LoseHealth(100);
                pObjA->Destroy();
            }
        }
    }
    else if (pObjA->GetTeam() == ETeam::Enemy && pObjB->GetTeam() == ETeam::Friendly)
    {
        if (pObjA->IsActive())
        {
            auto pObjAHealthComp = pObjA->GetComponent<HealthComponent>().lock();
            if (pObjAHealthComp)
            {
                pObjAHealthComp->LoseHealth(100);
                pObjB->Destroy();
            }
        }
    }
    // Nuke Drop
    else if (pObjA->GetTeam() == ETeam::Player && pObjB->GetTeam() == ETeam::NukeDrop)
    {
        if (pObjA->IsActive() && pObjB->IsActive())
        {
            mpGameManager->GetManager<EnemyAIManager>()->DestroyAllEnemies();
            pObjB->SetActiveState(false);
        }
    }
    else if (pObjA->GetTeam() == ETeam::NukeDrop && pObjB->GetTeam() == ETeam::Player)
    {
        if (pObjA->IsActive() && pObjB->IsActive())
        {
            mpGameManager->GetManager<EnemyAIManager>()->DestroyAllEnemies();
            pObjB->SetActiveState(false);
        }
    }
    // Life Drop
    else if (pObjA->GetTeam() == ETeam::Player && pObjB->GetTeam() == ETeam::LifeDrop)
    {
        if (pObjA->IsActive() && pObjB->IsActive())
        {
            auto pObjAHealthComp = pObjA->GetComponent<HealthComponent>().lock();
            if (pObjAHealthComp)
            {
                pObjAHealthComp->AddLife(1);
            }
            pObjB->Destroy();
        }
    }
    else if (pObjA->GetTeam() == ETeam::LifeDrop && pObjB->GetTeam() == ETeam::Player)
    {
        if (pObjA->IsActive() && pObjB->IsActive())
        {
            auto pObjBHealthComp = pObjB->GetComponent<HealthComponent>().lock();
            if (pObjBHealthComp)
            {
                pObjBHealthComp->AddLife(1);
            }
            pObjA->Destroy();
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------

void CollisionListener::EndContact(b2Contact * contact)
{
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------