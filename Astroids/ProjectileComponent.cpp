#include "AstroidsPrivate.h"
#include "ProjectileComponent.h"
#include <iostream>
#include <cmath>
#include <stack>
#include "imgui.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "BDConfig.h"
#include "PlayerManager.h"
#include "ResourceManager.h"

ProjectileComponent::ProjectileComponent(GameObject * pOwner)
	: GameComponent(pOwner)
	, mProjectiles()
	, mSpeed(700.f)
	, mCooldown(.2f)
	, mTimeSinceLastShot(1.f)
	, mLastUsedProjectile(EProjectile::GreenLaser)
	, mName("ProjectileComponent")
{

}

//------------------------------------------------------------------------------------------------------------------------

ProjectileComponent::~ProjectileComponent()
{
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

void ProjectileComponent::Shoot()
{
	auto * pProjectile = mpOwner->GetGameManager().CreateNewGameObject(ETeam::Friendly, mpOwner);

	auto pProjectileSpriteComponent = pProjectile->GetComponent<SpriteComponent>().lock();
	if (pProjectileSpriteComponent)
	{
		auto file = GetCorrectProjectileFile();
		ResourceId resourceId(file);
		auto pTexture = mpOwner->GetGameManager().GetManager<ResourceManager>()->GetTexture(resourceId);

		pProjectileSpriteComponent->SetSprite(pTexture, sf::Vector2f(1.05f, 1.05f));

		// Get ship's position, size
		sf::Vector2f playerPosition = mpOwner->GetPosition();
		sf::Vector2f playerSize = mpOwner->GetSize();

		// Calculate edge offset
		sf::Vector2f offset;
		if (mLastUsedProjectile == EProjectile::RedLaser)
		{
			offset = sf::Vector2f(playerSize.y / 2.f, 0);
		}
		else
		{
			offset = sf::Vector2f(-playerSize.y / 2.f, 0);
		}

		// Final spawn position: ship position + offset
		sf::Vector2f spawnPosition = playerPosition + offset;

		// Get mouse position relative to the window
		sf::Vector2i mousePos = sf::Mouse::getPosition(*GetGameObject().GetGameManager().mpWindow);
		sf::Vector2f mousePosition(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

		// Calculate direction vector
		sf::Vector2f direction = mousePosition - spawnPosition;

		// Normalize the direction vector
		float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
		if (length != 0)
		{
			direction /= length; // Normalize
		}

		// Calculate angle in degrees
		float angleDegrees = std::atan2(direction.y, direction.x) * (180.f / 3.14159265f);

		// Set projectile position and rotation
		pProjectileSpriteComponent->SetPosition(spawnPosition);
		pProjectileSpriteComponent->SetRotation(angleDegrees + 90.f); // Adjust rotation for sprite alignment

		// Add collision
		pProjectile->CreatePhysicsBody(&mpOwner->GetGameManager().GetPhysicsWorld(), pProjectile->GetSize(), true);
		auto pCollisionComponent = std::make_shared<CollisionComponent>(
			pProjectile,
			&mpOwner->GetGameManager().GetPhysicsWorld(),
			pProjectile->GetPhysicsBody(),
            pProjectile->GetSize(), 
            true
        );
		pProjectile->AddComponent(pCollisionComponent);

		// Add the projectile to the projectiles list
		mProjectiles.push_back({ pProjectile, 3.f, 15, direction });
	}
}

//------------------------------------------------------------------------------------------------------------------------

void ProjectileComponent::Update(float deltaTime)
{
	if (!mpOwner) return;

	mTimeSinceLastShot += deltaTime;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mTimeSinceLastShot >= mCooldown)
	{
		float rotationInDegrees = mpOwner->GetRotation(); // Assuming GetRotation() exists
		float rotationInRadians = rotationInDegrees * (3.14159f / 180.0f);

		sf::Vector2f direction(std::sin(rotationInRadians), -std::cos(rotationInRadians));
		Shoot();
		mTimeSinceLastShot = 0.0f;
	}

	UpdateProjectiles(deltaTime);
}

//------------------------------------------------------------------------------------------------------------------------

void ProjectileComponent::draw(sf::RenderTarget & target, sf::RenderStates states)
{
	
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
		if (projectile.pObject && !projectile.pObject->IsDestroyed())
		{
			sf::Vector2f currentPosition = projectile.pObject->GetPosition();
			sf::Vector2f newPosition = currentPosition + (projectile.direction * mSpeed * deltaTime);
			projectile.pObject->SetPosition(newPosition);

			projectile.lifespan -= deltaTime;
		}
	}

	mProjectiles.erase(
		std::remove_if(mProjectiles.begin(), mProjectiles.end(),
			[](Projectile & proj) {
				if (proj.pObject && !proj.pObject->IsDestroyed() && proj.lifespan <= 0.0f)
				{
					proj.pObject->Destroy(); // Destroy the object if its lifespan is over
				}
				// Remove if the object is destroyed or lifespan is exhausted
				return !proj.pObject || proj.pObject->IsDestroyed();
			}),
		mProjectiles.end());
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------