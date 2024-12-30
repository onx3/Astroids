#include "AstroidsPrivate.h"
#include "ControlledMovementComponent.h"
#include <cassert>
#include "GameObject.h"
#include "SpriteComponent.h"
#include "BDConfig.h"
#include "ResourceManager.h"

ControlledMovementComponent::ControlledMovementComponent(GameObject * pOwner)
    : GameComponent(pOwner)
    , mVelocity(3.f, 3.f)
    , mAcceleration(800.f)
    , mDeceleration(1000.f)
    , mMaxSpeed(300.f)
    , mVelocityX(0.f)
    , mVelocityY(0.f)
    , mName("ControlledMovementComponent")
    , mTilt(ESpriteTilt::Normal)
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

void ControlledMovementComponent::Update(float deltaTime)
{
    if (!mpOwner->IsActive())
    {
        return;
    }

    auto pSpriteComponent = GetGameObject().GetComponent<SpriteComponent>().lock();

    if (pSpriteComponent)
    {
        // Get current position, size, and window bounds
        auto position = pSpriteComponent->GetPosition();
        sf::Vector2f size(pSpriteComponent->GetWidth(), pSpriteComponent->GetHeight());
        sf::Vector2u windowSize = GetGameObject().GetGameManager().mpWindow->getSize();

        sf::Vector2f inputDirection = { 0.f, 0.f };


        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            inputDirection.y -= 1.f;
            if (mTilt != ESpriteTilt::Normal)
            {
                std::string file = "Art/Player.png";
                ResourceId resourceId(file);
                auto pTexture = mpOwner->GetGameManager().GetManager<ResourceManager>()->GetTexture(resourceId);
                if (pTexture)
                {
                    pSpriteComponent->SetSprite(pTexture, pSpriteComponent->GetSprite().getScale());
                }
                mTilt = ESpriteTilt::Normal;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            inputDirection.y += 1.f;
            if (mTilt != ESpriteTilt::Normal)
            {
                std::string file = "Art/Player.png";
                ResourceId resourceId(file);
                auto pTexture = mpOwner->GetGameManager().GetManager<ResourceManager>()->GetTexture(resourceId);
                if (pTexture)
                {
                    pSpriteComponent->SetSprite(pTexture, pSpriteComponent->GetSprite().getScale());
                }
                mTilt = ESpriteTilt::Normal;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            inputDirection.x -= 1.f;
            if (mTilt != ESpriteTilt::Left)
            {
                std::string file = "Art/PlayerLeft.png";
                ResourceId resourceId(file);
                auto pTexture = mpOwner->GetGameManager().GetManager<ResourceManager>()->GetTexture(resourceId);
                if (pTexture)
                {
                    pSpriteComponent->SetSprite(pTexture, pSpriteComponent->GetSprite().getScale());
                }
                mTilt = ESpriteTilt::Left;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            inputDirection.x += 1.f;
            if (mTilt != ESpriteTilt::Right)
            {
                std::string file = "Art/PlayerRight.png";
                ResourceId resourceId(file);
                auto pTexture = mpOwner->GetGameManager().GetManager<ResourceManager>()->GetTexture(resourceId);
                if (pTexture)
                {
                    pSpriteComponent->SetSprite(pTexture, pSpriteComponent->GetSprite().getScale());
                }
                mTilt = ESpriteTilt::Right;
            }
        }

        // Normalize input direction to prevent faster diagonal movement
        if (inputDirection.x != 0.f || inputDirection.y != 0.f)
        {
            inputDirection /= std::sqrt(inputDirection.x * inputDirection.x + inputDirection.y * inputDirection.y);
        }

        // Apply acceleration
        mVelocity += inputDirection * mAcceleration * deltaTime;

        // Clamp velocity to max speed
        float velocityLength = std::hypot(mVelocity.x, mVelocity.y);
        if (velocityLength > mMaxSpeed)
        {
            mVelocity = (mVelocity / velocityLength) * mMaxSpeed;
        }

        // Apply deceleration if no input
        if (inputDirection.x == 0)
        {
            mVelocity.x -= std::min(std::abs(mVelocity.x), mDeceleration * deltaTime) * (mVelocity.x > 0 ? 1 : -1);
        }
        if (inputDirection.y == 0)
        {
            mVelocity.y -= std::min(std::abs(mVelocity.y), mDeceleration * deltaTime) * (mVelocity.y > 0 ? 1 : -1);
        }

        // Update position
        position += mVelocity * deltaTime;

        // Boundary checking
        float halfWidth = size.x / 2.0f;
        float halfHeight = size.y / 2.0f;

        position.x = std::clamp(position.x, halfWidth, windowSize.x - halfWidth);
        position.y = std::clamp(position.y, halfHeight, windowSize.y - halfHeight);

        pSpriteComponent->SetPosition(position);

        // Rotate sprite towards mouse
        sf::RenderWindow * pWindow = GetGameObject().GetGameManager().mpWindow;
        sf::Vector2i mousePosition = sf::Mouse::getPosition(*pWindow);

        sf::Vector2f direction = sf::Vector2f(mousePosition) - position;
        float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159f;
        pSpriteComponent->SetRotation(angle + 90.f);
    }
}

//------------------------------------------------------------------------------------------------------------------------

std::string & ControlledMovementComponent::GetClassName()
{
    return mName;
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