#include "CollisionComponent.h"
#include "box2d/box2d.h"

CollisionComponent::CollisionComponent(GameObject * pOwner, b2World * pWorld, b2Body * pBody, sf::Vector2f size, bool isDynamic)
    : GameComponent(pOwner)
    , mpWorld(pWorld)
    , mpBody(pBody)
{
}

//------------------------------------------------------------------------------------------------------------------------

CollisionComponent::~CollisionComponent()
{
    mpWorld->DestroyBody(mpBody);
}

//------------------------------------------------------------------------------------------------------------------------

void CollisionComponent::Update()
{
    // Every frame set the physics body to be equal to the sprite
    float scale = 30.f;
    auto spritePos = mpOwner->GetPosition();
    b2Vec2 box2dPosition(spritePos.x / scale, spritePos.y / scale);
    auto rotation = mpOwner->GetRotation() * (b2_pi / 180.0f);;
    mpBody->SetTransform(box2dPosition, rotation);
}

//------------------------------------------------------------------------------------------------------------------------

bool CollisionComponent::IsColliding() const
{
    for (b2ContactEdge * contactEdge = mpBody->GetContactList(); contactEdge; contactEdge = contactEdge->next)
    {
        if (contactEdge->contact->IsTouching())
        {
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------