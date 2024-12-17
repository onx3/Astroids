#include "ProjectileComponent.h"
#include <iostream>
#include "SFML/Graphics.hpp"
#include "imgui.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "BDConfig.h"

ProjectileComponent::ProjectileComponent(GameObject * pOwner)
	: GameComponent(pOwner)
	, mProjectiles()
	, mSpeed(500.f)
	, mCooldown(.1f)
	, mTimeSinceLastShot(1.f)
	, mLastUsedProjectile(EProjectile::GreenLaser)
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

void ProjectileComponent::Shoot(sf::Vector2f direction)
{
	auto * pProjectile = new GameObject(&GetGameObject().GetGameManager());
	pProjectile->SetTeam(mpOwner->GetTeam()); // Optional, match team if implemented

	auto pProjectileSpriteComponent = pProjectile->GetComponent<SpriteComponent>().lock();
	if (pProjectileSpriteComponent)
	{
		pProjectileSpriteComponent->SetSprite(GetCorrectProjectileFile());
		sf::Vector2f playerPosition = mpOwner->GetPosition();
		sf::Vector2f playerSize = mpOwner->GetSize();
		sf::Vector2f projectilePosition = playerPosition + sf::Vector2f(0, -playerSize.y / 2);
		pProjectileSpriteComponent->SetPosition(projectilePosition);
		pProjectileSpriteComponent->SetRotation(mpOwner->GetRotation());
		auto pCollisionComponent = std::make_shared<CollisionComponent>(pProjectile, pProjectile->GetSize());
		pProjectile->AddComponent(pCollisionComponent);

		mProjectiles.push_back({ pProjectile, 3.f, 15, direction });
	}

	// Add the projectile to the GameManager's object list
	GetGameObject().GetGameManager().GetGameObjects().push_back(pProjectile);
}


//------------------------------------------------------------------------------------------------------------------------

void ProjectileComponent::Update()
{
	if (!mpOwner) return;

	float deltaTime = mpOwner->GetDeltaTime();
	mTimeSinceLastShot += deltaTime;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && mTimeSinceLastShot >= mCooldown)
	{
		float rotationInDegrees = mpOwner->GetRotation(); // Assuming GetRotation() exists
		float rotationInRadians = rotationInDegrees * (3.14159f / 180.0f);

		sf::Vector2f direction(std::sin(rotationInRadians), -std::cos(rotationInRadians));
		Shoot(direction);
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
		auto pCollisionComponent = projectile.pObject->GetComponent<CollisionComponent>().lock();

		if (pSpriteComponent && pCollisionComponent)
		{
			sf::Vector2f position = pSpriteComponent->GetPosition();
			position += projectile.direction * mSpeed * deltaTime;
			pSpriteComponent->SetPosition(position);

			auto & gameObjects = GetGameObject().GetGameManager().GetGameObjects();
			for (auto * pGameObject : gameObjects)
			{
				if (pGameObject != projectile.pObject && pGameObject->GetTeam() != projectile.pObject->GetTeam())
				{
					auto targetCollision = pGameObject->GetComponent<CollisionComponent>().lock();
					if (targetCollision && pCollisionComponent->CheckCollision(*targetCollision))
					{
						std::cout << "Projectile hit an object!\n";

						auto pHealthComponent = pGameObject->GetComponent<HealthComponent>().lock();
						if (pHealthComponent)
						{
							pHealthComponent->LooseHealth(projectile.damage);
							if (mpOwner->GetTeam() == ETeam::Player)
							{
								mpOwner->GetGameManager().GetScoreManager().AddScore(1000);
							}
						}

						if (!projectile.pObject->IsDestroyed())
						{
							projectile.pObject->Destroy();
							projectile.lifespan = 0.0f;
						}
						break;
					}
				}
			}
		}

		projectile.lifespan -= deltaTime;
		if (projectile.lifespan <= 0.0f && !projectile.pObject->IsDestroyed())
		{
			projectile.pObject->Destroy();
		}
	}

	// Remove expired projectiles
	mProjectiles.erase(
		std::remove_if(mProjectiles.begin(), mProjectiles.end(),
			[](const Projectile & proj) {
				return proj.pObject->IsDestroyed() || proj.lifespan <= 0.0f;
			}),
		mProjectiles.end());
}


//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------