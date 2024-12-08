#pragma once

#include "GameObject.h"
#include <SFML/Graphics.hpp>

class Player : public GameObject
{
public:
    Player();
    ~Player();

    void Update() override;

private:
    float mVelocityX;
    float mVelocityY;
    sf::Sprite mSprite;
    sf::Texture mTexture;
};