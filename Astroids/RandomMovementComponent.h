#pragma once
#include "GameComponent.h"
#include <SFML/System.hpp>
#include <random>

class RandomMovementComponent : public GameComponent
{
public:
	RandomMovementComponent(GameObject * pOwner);
	virtual ~RandomMovementComponent();

	void Update() override;

private:
	float mVelocity;
	int mDirection; // 1 is right -1 is left
	sf::Clock mClock;
	float mTimeToNextChange;

	std::default_random_engine mRandomEngine; // Random number generator
	std::uniform_real_distribution<float> mSpeedDist; // Random speed distribution
	std::uniform_real_distribution<float> mTimeDist;  // Random time distribution
};

