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
    , mName("ControlledMovementComponent")
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
    auto * body = GetGameObject().GetPhysicsBody();
    if (!mpOwner->IsActive() || !body)
    {
        return;
    }

    sf::Vector2f inputDirection = { 0.f, 0.f };

    // Handle input
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        inputDirection.y -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        inputDirection.y += 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        inputDirection.x -= 1.f;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        inputDirection.x += 1.f;
    }

    // Normalize the direction
    if (inputDirection.x != 0.f || inputDirection.y != 0.f)
    {
        inputDirection /= std::sqrt(inputDirection.x * inputDirection.x + inputDirection.y * inputDirection.y);
    }

    // Apply force to the physics body
    b2Vec2 force(inputDirection.x * mAcceleration, inputDirection.y * mAcceleration);
    body->ApplyForceToCenter(force, true);

    // Clamp velocity
    b2Vec2 velocity = body->GetLinearVelocity();
    float speed = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);
    if (speed > mMaxSpeed)
    {
        velocity *= mMaxSpeed / speed;
        body->SetLinearVelocity(velocity);
    }

    // Sync GameObject position and rotation with the physics body
    b2Vec2 position = body->GetPosition();
    mpOwner->SetPosition(sf::Vector2f(position.x, position.y));
    mpOwner->SetRotation(body->GetAngle() * 180.f / b2_pi);
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