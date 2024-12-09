#include "ControlledMovementComponent.h"
#include <SFML/Graphics.hpp>
#include <cassert>
#include "GameObject.h"
#include "SpriteComponent.h"

ControlledMovementComponent::ControlledMovementComponent(GameObject * pOwner)
	: GameComponent(pOwner)
    , mVelocityX(3.f)
	, mVelocityY(3.f)
{

}

//------------------------------------------------------------------------------------------------------------------------

ControlledMovementComponent::ControlledMovementComponent(GameObject * pOwner, float veloX, float veloY)
	: GameComponent(pOwner)
    , mVelocityX(veloX)
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
    auto spriteComponent = GetGameObject().GetComponent<SpriteComponent>().lock();
    
    if (spriteComponent)
    {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            spriteComponent->Move(0, -mVelocityY);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            spriteComponent->Move(0, mVelocityY);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            spriteComponent->Move(mVelocityX, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            spriteComponent->Move(-mVelocityX, 0);
        }
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