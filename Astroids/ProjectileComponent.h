#pragma once
#include "GameComponent.h"
#include <SFML/System/Vector2.hpp>

class ProjectileComponent : public GameComponent
{
	ProjectileComponent(GameObject * pOwner);
	void Shoot(sf::Vector2f direction);

	void Update() override;
};