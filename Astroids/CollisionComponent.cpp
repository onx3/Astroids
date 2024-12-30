#include "AstroidsPrivate.h"
#include "CollisionComponent.h"
#include "box2d/box2d.h"

CollisionComponent::CollisionComponent(GameObject * pOwner, b2World * pWorld, b2Body * pBody, sf::Vector2f size, bool isDynamic)
    : GameComponent(pOwner)
    , mpWorld(pWorld)
    , mpBody(pBody)
    , mSize(size)
{
    mpBody->SetSleepingAllowed(false);
}

//------------------------------------------------------------------------------------------------------------------------

CollisionComponent::~CollisionComponent()
{
    mpWorld->DestroyBody(mpBody);
}

//------------------------------------------------------------------------------------------------------------------------

void CollisionComponent::Update(float deltaTime)
{
    if (mpOwner->IsActive())
    {
        float scale = mpOwner->PIXELS_PER_METER;
        auto spritePos = mpOwner->GetPosition();
        b2Vec2 box2dPosition(spritePos.x / scale, spritePos.y / scale);
        auto rotation = mpOwner->GetRotation() * (b2_pi / 180.0f);

        // Only update if there's a difference
        if (box2dPosition != mpBody->GetPosition() || rotation != mpBody->GetAngle())
        {
            mpBody->SetTransform(box2dPosition, rotation);
        }
    }    
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------