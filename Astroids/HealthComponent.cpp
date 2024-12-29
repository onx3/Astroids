#include "AstroidsPrivate.h"
#include "HealthComponent.h"
#include "iostream"
#include "PlayerManager.h"
#include "ExplosionComponent.h"
#include "SpriteComponent.h"
#include "imgui.h"
#include <functional>

HealthComponent::HealthComponent(GameObject * pOwner, int initialHealth, int maxHealth, int lifeCount, int maxLives, float hitCooldown)
	: GameComponent(pOwner)
	, mHealth(initialHealth)
	, mMaxHealth(maxHealth)
	, mLifeCount(lifeCount)
	, mMaxLives(maxLives)
	, mHitCooldown(hitCooldown)
	, mTimeSinceLastHit(0.f)
	, mName("HealthComponent")
{
	mClock.restart();
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
	else
	{
		++mLifeCount;
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

void HealthComponent::SetDeathCallBack(std::function<void()> callback)
{
	mDeathCallback = callback;
}

//------------------------------------------------------------------------------------------------------------------------

void HealthComponent::SetLifeLostCallback(std::function<void()> callback)
{
	mLifeLostCallback = callback;
}

//------------------------------------------------------------------------------------------------------------------------

void HealthComponent::LoseLife()
{
	if (mLifeCount == 1)
	{
		--mLifeCount;
		if (mDeathCallback)
		{
			mDeathCallback();
		}
	}
	else if (mLifeCount > 0)
	{
		--mLifeCount;
		if (mLifeLostCallback)
		{
			mLifeLostCallback();
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------

void HealthComponent::Update()
{
	mTimeSinceLastHit = mClock.getElapsedTime().asSeconds();

	if (mHealth <= 0)
	{
		if (mLifeCount <= 1)
		{
			mpOwner->SetActiveState(false);
		}
		else
		{
			mHealth = mMaxHealth;
		}
		LoseLife();
	}

	if (mpOwner->IsDestroyed())
	{
		return;
	}

	if (mTimeSinceLastHit < mHitCooldown)
	{
		auto pSpriteComp = mpOwner->GetComponent<SpriteComponent>().lock();
		if (pSpriteComp)
		{
			float flicker = std::sin(mTimeSinceLastHit * 10.0f) * 127.5f + 127.5f;
			sf::Color spriteColor = pSpriteComp->GetSprite().getColor();
			spriteColor.a = static_cast<sf::Uint8>(flicker);
			pSpriteComp->GetSprite().setColor(spriteColor);
		}
	}
	else
	{
		auto pSpriteComp = mpOwner->GetComponent<SpriteComponent>().lock();
		if (pSpriteComp)
		{
			sf::Color spriteColor = pSpriteComp->GetSprite().getColor();
			spriteColor.a = 255;
			pSpriteComp->GetSprite().setColor(spriteColor);
		}
	}
}


//------------------------------------------------------------------------------------------------------------------------

void HealthComponent::DebugImGuiComponentInfo()
{
	ImGui::Text("Current amount of lives: %i", mLifeCount);
	ImGui::Text("Max Lives: %i", mMaxLives);
	ImGui::Text("Current Health: %i", mHealth);
	ImGui::Text("Max Health: %i", mMaxHealth);
}

//------------------------------------------------------------------------------------------------------------------------

std::string & HealthComponent::GetClassName()
{
	return mName;
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------
