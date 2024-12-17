#include "ControlledMovementComponent.h"
#include <SFML/Graphics.hpp>
#include <cassert>
#include "GameObject.h"
#include "SpriteComponent.h"
#include "BDConfig.h"

ControlledMovementComponent::ControlledMovementComponent(GameObject * pOwner)
    : GameComponent(pOwner)
    , mVelocity(3.f, 3.f)
    , mAcceleration(800.f)
    , mDeceleration(1000.f)
    , mMaxSpeed(300.f)
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

        sf::Vector2f inputDirection = { 0.f, 0.f };

        // Input direction based on key presses
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) inputDirection.y -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) inputDirection.y += 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) inputDirection.x -= 1.f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) inputDirection.x += 1.f;

        // Normalize input direction to ensure consistent speed in diagonals
        if (inputDirection.x != 0 || inputDirection.y != 0)
        {
            float length = std::sqrt(inputDirection.x * inputDirection.x + inputDirection.y * inputDirection.y);
            inputDirection /= length;
        }

        // Apply acceleration
        mVelocity += inputDirection * mAcceleration * GetGameObject().GetDeltaTime();

        // Clamp velocity to max speed
        if (std::hypot(mVelocity.x, mVelocity.y) > mMaxSpeed)
        {
            float length = std::hypot(mVelocity.x, mVelocity.y);
            mVelocity = (mVelocity / length) * mMaxSpeed;
        }

        // Apply deceleration if no input
        if (inputDirection.x == 0) mVelocity.x -= std::min(std::abs(mVelocity.x), mDeceleration * GetGameObject().GetDeltaTime()) * (mVelocity.x > 0 ? 1 : -1);
        if (inputDirection.y == 0) mVelocity.y -= std::min(std::abs(mVelocity.y), mDeceleration * GetGameObject().GetDeltaTime()) * (mVelocity.y > 0 ? 1 : -1);

        // Update position
        position += mVelocity * GetGameObject().GetDeltaTime();

        // Boundary checking
        position.x = std::clamp(position.x, 0.f, windowSize.x - size.x);
        position.y = std::clamp(position.y, 0.f, windowSize.y - size.y);

        pSpriteComponent->SetPosition(position);

        // Rotation towards the mouse
        sf::RenderWindow * window = GetGameObject().GetGameManager().mpWindow;
        sf::Vector2i mousePosition = sf::Mouse::getPosition(*window);
        sf::Vector2f spriteCenter = position + sf::Vector2f(size.x / 2.f, size.y / 2.f);

        sf::Vector2f direction = sf::Vector2f(mousePosition) - spriteCenter;
        float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159f; // Convert to degrees
        pSpriteComponent->SetRotation(angle + 90.f);
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