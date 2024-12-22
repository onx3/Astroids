#pragma once
#include "GameComponent.h"
#include <SFML/Graphics.hpp>

#include <box2d/box2d.h>

class CollisionComponent : public GameComponent
{
public:
    CollisionComponent(GameObject * pOwner, b2World * world, sf::Vector2f size, bool isDynamic);
    ~CollisionComponent();

    void Update() override;
    bool IsColliding() const;

private:
    b2Body * mBody;
    b2World * mWorld;
};

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------