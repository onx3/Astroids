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
	sf::Vector2f mDirection;
};

