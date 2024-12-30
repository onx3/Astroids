#pragma once
#include "AstroidsPrivate.h"
#include "GameComponent.h"
#include <functional>

class HealthComponent : public GameComponent
{
public:
	HealthComponent(GameObject * pOwner, int initialHealth, int maxHealth, int lifeCount, int maxLives, float hitCooldown = 0.f);

	int GetHealth() const;
	void AddHealth(int amount);
	void LoseHealth(int amount);

	int GetLives() const;
	void AddLife(int amount);
	void LoseLife();

	int GetMaxHealth() const;
	void AddMaxHealth(int amount);

	void SetDeathCallBack(std::function<void()> callback);
	void SetLifeLostCallback(std::function<void()> callback);

	virtual void Update(float deltaTime) override;

	virtual void DebugImGuiComponentInfo() override;

	virtual std::string & GetClassName() override;
private:
	int mHealth;
	int mMaxHealth;
	int mLifeCount;
	int mMaxLives;
	float mHitCooldown;
	float mTimeSinceLastHit;
	std::string mName;
	
	std::function<void()> mLifeLostCallback;
	std::function<void()> mDeathCallback;
};

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------