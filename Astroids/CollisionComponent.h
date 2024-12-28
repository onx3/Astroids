#pragma once
#include "GameComponent.h"
#include <SFML/Graphics.hpp>

#include <box2d/box2d.h>

class CollisionComponent : public GameComponent
{
public:
    CollisionComponent(GameObject * pOwner, b2World * pWorld, b2Body * pBody, sf::Vector2f size, bool isDynamic);
    ~CollisionComponent();

    void Update() override;

private:
    b2Body * mpBody;
    b2World * mpWorld;
    sf::Vector2f mSize;
};

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------