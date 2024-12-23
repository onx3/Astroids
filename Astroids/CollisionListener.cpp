#include "CollisionListener.h"
#include "GameObject.h"
#include "ProjectileComponent.h"

void CollisionListener::BeginContact(b2Contact * contact)
{
    GameObject * pObjectA = reinterpret_cast<GameObject *>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    GameObject * pObjectB = reinterpret_cast<GameObject *>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    if (pObjectA && pObjectB)
    {
        if (pObjectA->GetTeam() == ETeam::Friendly && pObjectB->GetTeam() == ETeam::Enemy)
        {
            std::cout << "BodyA Position: " << contact->GetFixtureA()->GetBody()->GetPosition().x << ", "
                << contact->GetFixtureA()->GetBody()->GetPosition().y << std::endl;
            std::cout << "BodyB Position: " << contact->GetFixtureB()->GetBody()->GetPosition().x << ", "
                << contact->GetFixtureB()->GetBody()->GetPosition().y << std::endl;

            //pObjectB->SetActiveState(false);
        }
    }
}

void CollisionListener::EndContact(b2Contact * contact)
{
}
