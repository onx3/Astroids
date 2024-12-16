#pragma once
#include "GameComponent.h"
class HealthComponent : public GameComponent
{
public:
	HealthComponent(GameObject * pOwner, int initialHealth, int maxHealth);

	void TakeDamage(int amount);

	int GetHealth() const;
	void AddHealth(int amount);
	void LooseHealth(int amount);

	int GetMaxHealth() const;
	void AddMaxHealth(int amount);

	void Update() override;

private:
	int mHealth;
	int mMaxHealth;
};

