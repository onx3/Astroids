#include "HealthComponent.h"

HealthComponent::HealthComponent(GameObject * pOwner, int initialHealth, int maxHealth)
	: GameComponent(mpOwner)
	, mHealth(initialHealth)
	, mMaxHealth(maxHealth)
{
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
	// Nothing to do right now, maybe regen stuff
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------
