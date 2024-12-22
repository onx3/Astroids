#include "CollisionListener.h"
#include "GameObject.h"
#include "ProjectileComponent.h"

void CollisionListener::BeginContact(b2Contact * contact)
{
    GameObject * objectA = reinterpret_cast<GameObject *>(contact->GetFixtureA()->GetBody()->GetUserData().pointer);
    GameObject * objectB = reinterpret_cast<GameObject *>(contact->GetFixtureB()->GetBody()->GetUserData().pointer);

    if (objectA && objectB)
    {
        // Example: Check for projectile collision
        if (objectA->HasComponent<ProjectileComponent>() || objectB->HasComponent<ProjectileComponent>())
        {
            // Handle projectile hit logic
        }
    }
}

void CollisionListener::EndContact(b2Contact * contact)
{
}
