#pragma once
#include "GameComponent.h"
#include <SFML/Graphics.hpp>

class CollisionComponent : public GameComponent
{
public:
	CollisionComponent(GameObject * pOwner, sf::Vector2f size);
	~CollisionComponent();

	sf::FloatRect GetBounds() const;

	bool CheckCollision(const CollisionComponent & other) const;

	void Update() override;

private:
	sf::Vector2f mSize;
};

