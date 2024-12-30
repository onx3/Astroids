#include "AstroidsPrivate.h"
#include "HealthComponent.h"
#include <iostream>
#include "PlayerManager.h"
#include "ExplosionComponent.h"
#include "SpriteComponent.h"
#include "imgui.h"
#include <functional>
#include <cmath>

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
}

//------------------------------------------------------------------------------------------------------------------------

int HealthComponent::GetHealth() const
{
    return mHealth;
}

//------------------------------------------------------------------------------------------------------------------------

void HealthComponent::AddHealth(int amount)
{
    mHealth += amount;
    if (mHealth > mMaxHealth)
    {
        mHealth = mMaxHealth;
    }
}

//------------------------------------------------------------------------------------------------------------------------

void HealthComponent::LoseHealth(int amount)
{
    if (mTimeSinceLastHit >= mHitCooldown)
    {
        mHealth -= amount;
        if (mHealth < 0)
        {
            mHealth = 0;
        }
        mTimeSinceLastHit = 0.0f; // Reset the hit cooldown timer
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
    mLifeCount = std::min(mLifeCount + amount, mMaxLives);
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

void HealthComponent::Update(float deltaTime)
{
    // Increment time since last hit
    mTimeSinceLastHit += deltaTime;

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

    // Handle invincibility flickering effect
    auto pSpriteComp = mpOwner->GetComponent<SpriteComponent>().lock();
    if (pSpriteComp)
    {
        if (mTimeSinceLastHit < mHitCooldown)
        {
            float flicker = std::sin(mTimeSinceLastHit * 10.0f) * 127.5f + 127.5f;
            sf::Color spriteColor = pSpriteComp->GetSprite().getColor();
            spriteColor.a = static_cast<sf::Uint8>(flicker);
            pSpriteComp->GetSprite().setColor(spriteColor);
        }
        else
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
