#include "CollisionComponent.h"
#include "box2d/box2d.h"

CollisionComponent::CollisionComponent(GameObject * pOwner, b2World * world, sf::Vector2f size, bool isDynamic)
    : GameComponent(pOwner), mWorld(world)
{
    b2BodyDef bodyDef;
    bodyDef.position.Set(pOwner->GetPosition().x, pOwner->GetPosition().y);
    bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
    mBody = mWorld->CreateBody(&bodyDef);

    b2PolygonShape box;
    box.SetAsBox(size.x / 2.0f, size.y / 2.0f);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &box;
    fixtureDef.density = isDynamic ? 1.0f : 0.0f;
    mBody->CreateFixture(&fixtureDef);
}

//------------------------------------------------------------------------------------------------------------------------

CollisionComponent::~CollisionComponent()
{
    mWorld->DestroyBody(mBody);
}

//------------------------------------------------------------------------------------------------------------------------

void CollisionComponent::Update()
{
    b2Vec2 position = mBody->GetPosition();
    mpOwner->SetPosition(sf::Vector2f(position.x, position.y));
    mpOwner->SetRotation(mBody->GetAngle() * 180.0f / b2_pi);
}

//------------------------------------------------------------------------------------------------------------------------

bool CollisionComponent::IsColliding() const
{
    for (b2ContactEdge * contactEdge = mBody->GetContactList(); contactEdge; contactEdge = contactEdge->next)
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