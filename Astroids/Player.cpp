#include "Player.h"

Player::Player()
    : GameObject(), mVelocityX(3.f), mVelocityY(3.f)
{
    SetTexture("Art/player.png");
    SetOriginToCenter();
}

//------------------------------------------------------------------------------------------------------------------------

Player::~Player()
{
}

//------------------------------------------------------------------------------------------------------------------------

void Player::Update()
{
    for (auto & component : mComponents)
    {
        component->Update();
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        Move(0, -mVelocityY);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        Move(0, mVelocityY);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        Move(mVelocityX, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        Move(-mVelocityX, 0);
    }
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------