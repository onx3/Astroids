#include "HealthComponent.h"

HealthComponent::HealthComponent(GameObject * pOwner, int initialHealth)
	: GameComponent(mpOwner)
	, mHealth(initialHealth)
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

void HealthComponent::Update()
{
	// Nothing to do right now, maybe regen stuff
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------
