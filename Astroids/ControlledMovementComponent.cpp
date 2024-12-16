#include "ControlledMovementComponent.h"
#include <SFML/Graphics.hpp>
#include <cassert>
#include "GameObject.h"
#include "SpriteComponent.h"
#include "BDConfig.h"

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
    auto pSpriteComponent = GetGameObject().GetComponent<SpriteComponent>().lock();
    
    if (pSpriteComponent)
    {
        auto position = pSpriteComponent->GetPosition();
        sf::Vector2f size(pSpriteComponent->GetWidth(), pSpriteComponent->GetHeight());

        sf::Vector2u windowSize = GetGameObject().GetGameManager().mpWindow->getSize();

        // Move up
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && position.y > 0)
        {
            pSpriteComponent->Move(0, -mVelocityY);
        }

        // Move down
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && position.y + size.y < windowSize.y)
        {
            pSpriteComponent->Move(0, mVelocityY);
        }

        // Move right
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && position.x + size.x < windowSize.x)
        {
            pSpriteComponent->Move(mVelocityX, 0);
        }

        // Move left
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && position.x > 0)
        {
            pSpriteComponent->Move(-mVelocityX, 0);
        }

        // Rotate right (clockwise)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        {
            pSpriteComponent->RotateClockwise();
        }

        // Rotate left (counter-clockwise)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            pSpriteComponent->RotateCounterClockwise();
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