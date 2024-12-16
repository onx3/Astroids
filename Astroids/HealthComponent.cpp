#include "HealthComponent.h"
#include "iostream"

HealthComponent::HealthComponent(GameObject * pOwner, int initialHealth, int maxHealth)
	: GameComponent(pOwner)
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

void HealthComponent::LooseHealth(int amount)
{
	mHealth -= amount;
	if (mHealth <= 0)
	{
		mpOwner->Destroy();
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
	/*if (mHealth <= 0)
	{
		mpOwner->Destroy();
	}*/
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------
