#include "ControlledMovementComponent.h"
#include <SFML/Graphics.hpp>
#include <cassert>
#include "GameObject.h"
#include "SpriteComponent.h"

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
    auto spriteComponent = GetGameObject().GetComponents<SpriteComponent>();
    assert(spriteComponent.size() == 1);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        spriteComponent[0]->Move(0, -mVelocityY);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        spriteComponent[0]->Move(0, mVelocityY);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        spriteComponent[0]->Move(mVelocityX, 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        spriteComponent[0]->Move(-mVelocityX, 0);
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