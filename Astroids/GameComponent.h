#pragma once

#include "SFML/Graphics.hpp"

class GameObject;

class GameComponent
{
public:
    explicit GameComponent(GameObject * pOwner);
    virtual ~GameComponent() = default;

    void SetOwner(GameObject * owner);

    GameObject & GetGameObject() const;

    virtual void Update() = 0;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const;
protected:

    GameObject * mpOwner = nullptr;
};