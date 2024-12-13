#include "RandomMovementComponent.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "GameManager.h"
#include <SFML/Graphics.hpp>

RandomMovementComponent::RandomMovementComponent(GameObject * pOwner)
	: GameComponent(pOwner)
	, mVelocity(100.f)
	, mDirection(1)
	, mTimeToNextChange(1.0f)
	, mSpeedDist(50.f, 200.f)
	, mTimeDist(1.f, 3.f)
{
	std::random_device rd; // Initialize random engine
	mRandomEngine.seed(rd());
}

RandomMovementComponent::~RandomMovementComponent()
{

}

void RandomMovementComponent::Update()
{
    auto spriteComponent = GetGameObject().GetComponent<SpriteComponent>().lock();

    if (spriteComponent)
    {
        // Get current position
        sf::Vector2f position = spriteComponent->GetPosition();
        sf::Vector2f size(spriteComponent->GetWidth(), spriteComponent->GetHeight());
        sf::Vector2u windowSize = GetGameObject().GetGameManager().mpWindow->getSize();

        // Update position based on direction and velocity
        float deltaX = mVelocity * mDirection * GetGameObject().GetDeltaTime(); // Assuming GetDeltaTime() gives elapsed time
        position.x += deltaX;

        // Bounds checking: Reverse direction if hitting the screen edges
        if (position.x < 0 || position.x + size.x > windowSize.x)
        {
            mDirection *= -1; // Reverse direction
            position.x = std::max(0.f, std::min(position.x, float(windowSize.x - size.x))); // Clamp position
        }

        spriteComponent->SetPosition(position);

        // Check if it's time to change direction
        if (mClock.getElapsedTime().asSeconds() > mTimeToNextChange)
        {
            mDirection = (mRandomEngine() % 2 == 0) ? 1 : -1; // Randomly choose direction
            mVelocity = mSpeedDist(mRandomEngine); // Random speed
            mTimeToNextChange = mTimeDist(mRandomEngine); // Random time to next change
            mClock.restart(); // Reset clock
        }
    }
}
