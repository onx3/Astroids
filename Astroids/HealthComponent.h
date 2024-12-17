#pragma once
#include "GameComponent.h"
class HealthComponent : public GameComponent
{
public:
	HealthComponent(GameObject * pOwner, int initialHealth, int maxHealth, int lifeCount, int maxLives, float hitCooldown = 0.f);

	void TakeDamage(int amount);

	int GetHealth() const;
	void AddHealth(int amount);
	void LooseHealth(int amount);

	int GetLives() const;
	void AddLife(int amount);

	int GetMaxHealth() const;
	void AddMaxHealth(int amount);

	void Update() override;

private:
	int mHealth;
	int mMaxHealth;
	int mLifeCount;
	int mMaxLives;
	float mHitCooldown;
	float mTimeSinceLastHit;

	sf::Clock mClock;
};

