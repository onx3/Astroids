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
    , mVelocityX(0.f)
    , mVelocityY(0.f)
{
}

//------------------------------------------------------------------------------------------------------------------------

ControlledMovementComponent::ControlledMovementComponent(GameObject * pOwner, float veloX, float veloY)
    : GameComponent(pOwner)
    , mVelocity(3.f, 3.f)
    , mAcceleration(800.f)
    , mDeceleration(1000.f)
    , mMaxSpeed(300.f)
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
        // Get current position, size, and window bounds
        auto position = pSpriteComponent->GetPosition();
        sf::Vector2f size(pSpriteComponent->GetWidth(), pSpriteComponent->GetHeight());
        sf::Vector2u windowSize = GetGameObject().GetGameManager().mpWindow->getSize();

        sf::Vector2f inputDirection = { 0.f, 0.f };

        // Input direction based on key presses
        static std::string currentSprite = "Art/Player.png";

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            inputDirection.y -= 1.f;
            if (currentSprite != "Art/Player.png")
            {
                pSpriteComponent->SetSprite("Art/Player.png", pSpriteComponent->GetSprite().getScale());
                currentSprite = "Art/Player.png";
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            inputDirection.y += 1.f;
            if (currentSprite != "Art/Player.png")
            {
                pSpriteComponent->SetSprite("Art/Player.png", pSpriteComponent->GetSprite().getScale());
                currentSprite = "Art/Player.png";
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            inputDirection.x -= 1.f;
            if (currentSprite != "Art/PlayerLeft.png")
            {
                pSpriteComponent->SetSprite("Art/PlayerLeft.png", pSpriteComponent->GetSprite().getScale());
                currentSprite = "Art/PlayerLeft.png";
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            inputDirection.x += 1.f;
            if (currentSprite != "Art/PlayerRight.png")
            {
                pSpriteComponent->SetSprite("Art/PlayerRight.png", pSpriteComponent->GetSprite().getScale());
                currentSprite = "Art/PlayerRight.png";
            }
        }

        // Normalize input direction to prevent faster diagonal movement
        if (inputDirection.x != 0.f || inputDirection.y != 0.f)
        {
            inputDirection /= std::sqrt(inputDirection.x * inputDirection.x + inputDirection.y * inputDirection.y);
        }

        // Apply acceleration
        mVelocity += inputDirection * mAcceleration * GetGameObject().GetDeltaTime();

        // Clamp velocity to max speed
        float velocityLength = std::hypot(mVelocity.x, mVelocity.y);
        if (velocityLength > mMaxSpeed)
        {
            mVelocity = (mVelocity / velocityLength) * mMaxSpeed;
        }

        // Apply deceleration if no input
        if (inputDirection.x == 0)
        {
            mVelocity.x -= std::min(std::abs(mVelocity.x), mDeceleration * GetGameObject().GetDeltaTime()) * (mVelocity.x > 0 ? 1 : -1);
        }
        if (inputDirection.y == 0)
        {
            mVelocity.y -= std::min(std::abs(mVelocity.y), mDeceleration * GetGameObject().GetDeltaTime()) * (mVelocity.y > 0 ? 1 : -1);
        }

        // Update position
        position += mVelocity * GetGameObject().GetDeltaTime();

        // Boundary checking (adjust for center origin)
        float halfWidth = size.x / 2.0f;
        float halfHeight = size.y / 2.0f;

        position.x = std::clamp(position.x, halfWidth, windowSize.x - halfWidth);
        position.y = std::clamp(position.y, halfHeight, windowSize.y - halfHeight);

        pSpriteComponent->SetPosition(position);

        // Rotate sprite towards mouse
        sf::RenderWindow * pWindow = GetGameObject().GetGameManager().mpWindow;
        sf::Vector2i mousePosition = sf::Mouse::getPosition(*pWindow);

        sf::Vector2f direction = sf::Vector2f(mousePosition) - position; // Centered position
        float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159f; // Convert to degrees
        pSpriteComponent->SetRotation(angle + 90.f);
    }
}


//------------------------------------------------------------------------------------------------------------------------

std::string ControlledMovementComponent::GetClassName()
{
    return "ControlledMovementComponent";
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