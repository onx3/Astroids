#pragma once
#include "GameComponent.h"
#include <SFML/System/Vector2.hpp>

class ProjectileComponent : public GameComponent
{
	void Shoot(sf::Vector2f direction);

	void Update() override;
};