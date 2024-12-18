#include "HealthComponent.h"
#include "iostream"
#include "PlayerManager.h"
#include "ExplosionComponent.h"
#include "SpriteComponent.h"
#include "imgui.h"

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
			mpOwner->SetActiveState(false);
			if (!mpOwner->GetComponent<ExplosionComponent>().lock())
			{
				auto explosionComp = std::make_shared<ExplosionComponent>(
					mpOwner, "Art/explosion.png", 32, 32, 7, 0.1f);
				mpOwner->AddComponent(explosionComp);
			}

			auto explosionComp = mpOwner->GetComponent<ExplosionComponent>().lock();
			if (explosionComp && explosionComp->IsAnimationFinished())
			{
				mpOwner->Destroy();
			}
		}
		else
		{
			mHealth = mMaxHealth;
			--mLifeCount;
		}
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
			// Create a flickering effect using sine wave
			float flicker = std::sin(mTimeSinceLastHit * 10.0f) * 127.5f + 127.5f; // Range [0, 255]
			sf::Color spriteColor = pSpriteComp->GetSprite().getColor();
			spriteColor.a = static_cast<sf::Uint8>(flicker); // Set alpha transparency
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
