#pragma once
#include "GameComponent.h"
class HealthComponent : public GameComponent
{
public:
	HealthComponent(int initialHealth);

	void TakeDamage(int amount);

	int GetHealth() const;

	void Update() override;

private:
	int mHealth;
};

