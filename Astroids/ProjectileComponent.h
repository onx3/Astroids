#pragma once
#include "GameComponent.h"
#include <SFML/System/Vector2.hpp>
#include "GameObject.h"

enum class EProjectile
{
	RedLaser,
	GreenLaser
};

class ProjectileComponent : public GameComponent
{
public:
	ProjectileComponent(GameObject * pOwner);
	~ProjectileComponent();

	std::string GetCorrectProjectileFile();

	void Shoot();

	void Update() override;

	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

	void DebugImGuiComponentInfo() override;

private:
	void UpdateProjectiles(float deltaTime);

	struct Projectile
	{
		GameObject * pObject;
		float lifespan;
		int damage;
		sf::Vector2f direction;
	};

	std::vector<Projectile> mProjectiles;

	float mSpeed;
	float mCooldown;
	float mTimeSinceLastShot;

	EProjectile mLastUsedProjectile;
};