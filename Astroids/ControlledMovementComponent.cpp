#include "ControlledMovementComponent.h"
#include <SFML/Graphics.hpp>
#include "GameObject.h"

ControlledMovementComponent::ControlledMovementComponent()
	: mVelocityX(3.f)
	, mVelocityY(3.f)
{

}

//------------------------------------------------------------------------------------------------------------------------

ControlledMovementComponent::ControlledMovementComponent(float veloX, float veloY)
	: mVelocityX(veloX)
	, mVelocityY(veloY)
{
}

//------------------------------------------------------------------------------------------------------------------------

ControlledMovementComponent::~ControlledMovementComponent()
{
}

//------------------------------------------------------------------------------------------------------------------------

void ControlledMovementComponent::Update()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        GetGameObject().Move(0, -mVelocityY);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        GetGameObject().Move(0, mVelocityY);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        GetGameObject().Move(mVelocityX, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        GetGameObject().Move(-mVelocityX, 0);
    }
}

//------------------------------------------------------------------------------------------------------------------------

void ControlledMovementComponent::SetVelocityX(float velo)
{
	mVelocityX = velo;
}

//------------------------------------------------------------------------------------------------------------------------

void ControlledMovementComponent::SetVelocityY(float velo)
{
	mVelocityY = velo;
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------