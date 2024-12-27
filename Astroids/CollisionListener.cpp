#include "CollisionListener.h"
#include "GameObject.h"
#include "ProjectileComponent.h"

void CollisionListener::BeginContact(b2Contact * contact)
{
    GameObject * pObjectA = reinterpret_cast<GameObject *>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    GameObject * pObjectB = reinterpret_cast<GameObject *>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    if (pObjectA && pObjectB)
    {
        //if (pObjectA->GetTeam() != ETeam::Player && pObjectB->GetTeam() != ETeam::Player)
        {
            HandleCollision(pObjectA, pObjectB);
        }
    }
    else
    {
        int ii = 0;
        ++ii;
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
        std::cout << "Player collided with Enemy\n";
        //pObjB->SetActiveState(false);
    }
    else if (pObjA->GetTeam() == ETeam::Enemy && pObjB->GetTeam() == ETeam::Player)
    {
        std::cout << "Enemy collided with Player\n";
        //pObjA->SetActiveState(false);
    }
    else if (pObjA->GetTeam() == ETeam::Friendly && pObjB->GetTeam() == ETeam::Enemy)
    {
        std::cout << "Projectile collided with Enemy\n";
        //pObjA->SetActiveState(false);
    }
    else if (pObjA->GetTeam() == ETeam::Enemy && pObjB->GetTeam() == ETeam::Friendly)
    {
        std::cout << "Projectile collided with Enemy\n";
        //pObjA->SetActiveState(false);
    }
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------