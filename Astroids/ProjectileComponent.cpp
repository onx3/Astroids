#include "ProjectileComponent.h"

#include <iostream>
#include "SFML/Graphics.hpp"

#include "imgui.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "BDConfig.h"

ProjectileComponent::ProjectileComponent(GameObject * pOwner)
	: GameComponent(pOwner)
	, mProjectiles()
	, mDirection({.0f, -1.f})
	, mSpeed(500.f)
	, mCooldown(.5f)
	, mTimeSinceLastShot(0.f)
	, mLastUsedProjectile(EProjectile::GreenLaser)
{

}

//------------------------------------------------------------------------------------------------------------------------

ProjectileComponent::~ProjectileComponent()
{
	for (auto & pair : mProjectiles)
	{
		delete pair.pObject;
	}
}

//------------------------------------------------------------------------------------------------------------------------

std::string ProjectileComponent::GetCorrectProjectileFile()
{
	if (mLastUsedProjectile == EProjectile::GreenLaser)
	{
		mLastUsedProjectile = EProjectile::RedLaser;
		return "Art/laserRed.png";
	}
	else
	{
		mLastUsedProjectile = EProjectile::GreenLaser;
		return "Art/laserGreen.png";
	}
}

//------------------------------------------------------------------------------------------------------------------------

void ProjectileComponent::Shoot(sf::Vector2f direction)
{
	auto * pProjectile = new GameObject(&GetGameObject().GetGameManager());
	std::cout << "Created a new projectile\n";
	sf::Vector2f playerPosition;
	sf::Vector2f playerSize;
	sf::Vector2f projectilePosition;

	auto pProjectileSpriteComponent = pProjectile->GetComponent<SpriteComponent>().lock();
	if (pProjectileSpriteComponent)
	{
		pProjectileSpriteComponent->SetSprite(GetCorrectProjectileFile());

		playerPosition = mpOwner->GetPosition();
		playerSize = mpOwner->GetSize();
		projectilePosition = playerPosition + sf::Vector2f(playerSize.x / 2, -playerSize.y / 2);

		pProjectileSpriteComponent->SetPosition(projectilePosition);

		mProjectiles.push_back({ pProjectile, 3.f });
	}
}

//------------------------------------------------------------------------------------------------------------------------

void ProjectileComponent::Update()
{
	if (!mpOwner) return;

	float deltaTime = mpOwner->GetDeltaTime();
	mTimeSinceLastShot += deltaTime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mTimeSinceLastShot >= mCooldown)
	{
		Shoot(mDirection);
		mTimeSinceLastShot = 0.0f;
	}

	UpdateProjectiles(deltaTime);
}

//------------------------------------------------------------------------------------------------------------------------

void ProjectileComponent::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	for (auto & projectile : mProjectiles)
	{
		auto spriteComponentToDraw = projectile.pObject->GetComponent<SpriteComponent>().lock();
		target.draw(spriteComponentToDraw->GetSprite());
	}
}

//------------------------------------------------------------------------------------------------------------------------

void ProjectileComponent::DebugImGuiComponentInfo()
{
#if IMGUI_ENABLED()
	for (auto & projectile : mProjectiles)
	{
		ImGui::Text("This is a projectile game object");
	}
#endif
}

//------------------------------------------------------------------------------------------------------------------------

void ProjectileComponent::UpdateProjectiles(float deltaTime)
{
	for (auto & projectile : mProjectiles)
	{
		auto pSpriteComponent = projectile.pObject->GetComponent<SpriteComponent>().lock();
		if (pSpriteComponent)
		{

			// Move the projectile
			sf::Vector2f position = pSpriteComponent->GetPosition();
			position += mDirection * mSpeed * deltaTime;
			pSpriteComponent->SetPosition(position);
		}

		// Decrease the lifespan
		projectile.lifespan -= deltaTime;
	}

	// Remove expired projectiles
	mProjectiles.erase(
		std::remove_if(
			mProjectiles.begin(),
			mProjectiles.end(),
			[](const Projectile & proj) {
				if (proj.lifespan <= 0.0f)
				{
					std::cout << "Deleting a projectile object\n";
					delete proj.pObject; // Clean up dynamically allocated memory
					return true;
				}
				return false;
			}),
		mProjectiles.end());
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------