#include "RandomMovementComponent.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "GameManager.h"
#include <SFML/Graphics.hpp>
#include <random>

//------------------------------------------------------------------------------------------------------------------------

RandomMovementComponent::RandomMovementComponent(GameObject * pOwner)
    : GameComponent(pOwner)
    , mVelocity(200.f)
{
    // Random number generator setup
    static std::random_device rd;
    static std::mt19937 rng(rd());
    static std::uniform_real_distribution<float> randomDirection(-1.0f, 1.0f);

    auto pSpriteComponent = GetGameObject().GetComponent<SpriteComponent>().lock();
    if (pSpriteComponent)
    {
        // Generate a random direction vector
        sf::Vector2f randomVector(randomDirection(rng), randomDirection(rng));

        // Normalize the random direction to make it a unit vector
        float magnitude = std::sqrt(randomVector.x * randomVector.x + randomVector.y * randomVector.y);
        if (magnitude != 0)
        {
            mDirection = randomVector / magnitude; // Set normalized direction
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------

RandomMovementComponent::~RandomMovementComponent()
{
    // Destructor logic (nothing to clean up here)
}

//------------------------------------------------------------------------------------------------------------------------

void RandomMovementComponent::Update()
{
    if (mpOwner->IsActive())
    {
        auto spriteComponent = GetGameObject().GetComponent<SpriteComponent>().lock();

        if (spriteComponent)
        {
            // Get current position and window bounds
            sf::Vector2f position = spriteComponent->GetPosition();
            sf::Vector2u windowSize = GetGameObject().GetGameManager().mpWindow->getSize();
            sf::Vector2f size(spriteComponent->GetWidth(), spriteComponent->GetHeight());

            // Update position based on direction and velocity
            position += mDirection * mVelocity * GetGameObject().GetDeltaTime();

            if (position.x < 0 || position.x + size.x > windowSize.x)
            {
                mDirection.x *= -1; // Reverse horizontal direction
                position.x = std::max(0.f, std::min(position.x, windowSize.x - size.x)); // Clamp position to screen bounds
            }
            if (position.y < 0 || position.y + size.y > windowSize.y)
            {
                mDirection.y *= -1; // Reverse vertical direction
                position.y = std::max(0.f, std::min(position.y, windowSize.y - size.y)); // Clamp position to screen bounds
            }

            // Apply the updated position
            spriteComponent->SetPosition(position);
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------
