#pragma once
#include "AstroidsPrivate.h"
#include "GameComponent.h"
#include <box2d/box2d.h>

class CollisionComponent : public GameComponent
{
public:
    CollisionComponent(GameObject * pOwner, b2World * pWorld, b2Body * pBody, sf::Vector2f size, bool isDynamic);
    ~CollisionComponent();

    void Update(float deltaTime) override;

private:
    b2Body * mpBody;
    b2World * mpWorld;
    sf::Vector2f mSize;
};

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------