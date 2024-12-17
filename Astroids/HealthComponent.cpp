#include "HealthComponent.h"
#include "iostream"

HealthComponent::HealthComponent(GameObject * pOwner, int initialHealth, int maxHealth, int lifeCount, int maxLives, float hitCooldown)
	: GameComponent(pOwner)
	, mHealth(initialHealth)
	, mMaxHealth(maxHealth)
	, mLifeCount(lifeCount)
	, mMaxLives(maxLives)
	, mHitCooldown(hitCooldown)
	, mTimeSinceLastHit(0.f)
{
	mClock.restart();
}

//------------------------------------------------------------------------------------------------------------------------

void HealthComponent::TakeDamage(int amount)
{
	mHealth -= amount;
}

//------------------------------------------------------------------------------------------------------------------------

int HealthComponent::GetHealth() const
{
	return mHealth;
}

//------------------------------------------------------------------------------------------------------------------------

void HealthComponent::AddHealth(int amount)
{
	int newHealth = mHealth += amount;
	if (mHealth += amount > mMaxHealth)
	{
		mHealth = mMaxHealth;
	}
	else
	{
		mHealth = newHealth;
	}
}

//------------------------------------------------------------------------------------------------------------------------

void HealthComponent::LooseHealth(int amount)
{
	mTimeSinceLastHit = mClock.getElapsedTime().asSeconds();
	if (mTimeSinceLastHit >= mHitCooldown)
	{
		mHealth -= amount;
		if (mHealth < 0)
		{
			mHealth = 0;
		}

		mClock.restart();
	}
}

//------------------------------------------------------------------------------------------------------------------------

int HealthComponent::GetLives() const
{
	return mLifeCount;
}

//------------------------------------------------------------------------------------------------------------------------

void HealthComponent::AddLife(int amount)
{
	if (mLifeCount + amount >= mMaxLives)
	{
		mLifeCount = mMaxLives;
	}
}

//------------------------------------------------------------------------------------------------------------------------

int HealthComponent::GetMaxHealth() const
{
	return mMaxHealth;
}

//------------------------------------------------------------------------------------------------------------------------

void HealthComponent::AddMaxHealth(int amount)
{
	mMaxHealth += amount;
}

//------------------------------------------------------------------------------------------------------------------------

void HealthComponent::Update()
{
	mTimeSinceLastHit = mClock.getElapsedTime().asSeconds();
	if (mHealth <= 0)
	{
		if (mLifeCount <= 1)
		{
			mpOwner->Destroy();
		}
		else
		{
			mHealth = mMaxHealth;
			--mLifeCount;
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------
