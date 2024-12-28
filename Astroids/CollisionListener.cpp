#include "CollisionListener.h"
#include "GameObject.h"
#include "ProjectileComponent.h"
#include "HealthComponent.h"

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

void CollisionListener::EndContact(b2Contact * contact)
{
}

//------------------------------------------------------------------------------------------------------------------------

void CollisionListener::HandleCollision(GameObject * pObjA, GameObject * pObjB)
{
    if (pObjA->GetTeam() == ETeam::Player && pObjB->GetTeam() == ETeam::Enemy)
    {
        auto pHealthComp = pObjA->GetComponent<HealthComponent>().lock();
        if (pHealthComp)
        {
            if (pObjB->IsActive())
            {
                pHealthComp->LooseHealth(100);
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
                pHealthComp->LooseHealth(100);
            }
        }
    }
    else if (pObjA->GetTeam() == ETeam::Friendly && pObjB->GetTeam() == ETeam::Enemy)
    {
        if (pObjB->IsActive())
        {
            auto pObjBHealthComp = pObjB->GetComponent<HealthComponent>().lock();
            if (pObjBHealthComp)
            {
                pObjBHealthComp->LooseHealth(100);
                pObjA->SetActiveState(false);
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
                pObjAHealthComp->LooseHealth(100);
                pObjB->SetActiveState(false);
                pObjB->Destroy();
            }
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------