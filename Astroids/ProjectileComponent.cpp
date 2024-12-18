#include "ProjectileComponent.h"
#include <iostream>
#include <cmath>
#include <stack>
#include "SFML/Graphics.hpp"
#include "imgui.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "BDConfig.h"
#include "PlayerManager.h"

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

void ProjectileComponent::Shoot()
{
	auto * pProjectile = mpOwner->GetGameManager().CreateNewGameObject(mpOwner->GetTeam(), mpOwner);

	auto pProjectileSpriteComponent = pProjectile->GetComponent<SpriteComponent>().lock();
	if (pProjectileSpriteComponent)
	{
		pProjectileSpriteComponent->SetSprite(GetCorrectProjectileFile());

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
		auto pCollisionComponent = std::make_shared<CollisionComponent>(pProjectile, pProjectile->GetSize());
		pProjectile->AddComponent(pCollisionComponent);

		// Add the projectile to the projectiles list
		mProjectiles.push_back({ pProjectile, 3.f, 15, direction });
	}
}



//------------------------------------------------------------------------------------------------------------------------

void ProjectileComponent::Update()
{
	if (!mpOwner) return;

	float deltaTime = mpOwner->GetDeltaTime();
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
	auto & gameManager = mpOwner->GetGameManager();

	// Iterate through all projectiles
	for (auto & projectile : mProjectiles)
	{
		// Update projectile position
		auto pSpriteComponent = projectile.pObject->GetComponent<SpriteComponent>().lock();
		auto pCollisionComponent = projectile.pObject->GetComponent<CollisionComponent>().lock();

		if (pSpriteComponent && pCollisionComponent)
		{
			sf::Vector2f position = pSpriteComponent->GetPosition();
			position += projectile.direction * mSpeed * deltaTime; // Move the projectile
			pSpriteComponent->SetPosition(position);

			// Check for collisions in the GameObject hierarchy
			CheckCollisionsForProjectile(gameManager.GetRootGameObject(), projectile, pCollisionComponent);
		}

		// Check lifespan
		projectile.lifespan -= deltaTime;
		if (projectile.lifespan <= 0.0f && !projectile.pObject->IsDestroyed())
		{
			projectile.pObject->Destroy(); // Destroy projectile if lifespan expired
		}
	}

	// Remove destroyed or expired projectiles
	mProjectiles.erase(
		std::remove_if(mProjectiles.begin(), mProjectiles.end(),
			[](const Projectile & proj) {
				return proj.pObject->IsDestroyed() || proj.lifespan <= 0.0f;
			}),
		mProjectiles.end());
}

//------------------------------------------------------------------------------------------------------------------------

void ProjectileComponent::CheckCollisionsForProjectile(GameObject* root, Projectile & projectile, std::shared_ptr<CollisionComponent> pProjectileCollision)
{
    if (!root) return;

    std::stack<GameObject *> stack;
    stack.push(root);

    while (!stack.empty())
    {
        GameObject* current = stack.top();
        stack.pop();

        // Skip self and same-team GameObjects
        if (current != projectile.pObject && current->GetTeam() != projectile.pObject->GetTeam())
        {
            auto targetCollision = current->GetComponent<CollisionComponent>().lock();
            if (targetCollision && pProjectileCollision->CheckCollision(*targetCollision))
            {
                auto pHealthComponent = current->GetComponent<HealthComponent>().lock();
                if (pHealthComponent)
                {
                    // Deal damage
                    pHealthComponent->LooseHealth(projectile.damage);

                    // Award score if owner is the player
                    auto* pPlayerManager = mpOwner->GetGameManager().GetManager<PlayerManager>();
                    auto* pPlayer = pPlayerManager ? pPlayerManager->GetPlayers()[0] : nullptr;

                    if (pPlayer && mpOwner == pPlayer)
                    {
                        mpOwner->GetGameManager().GetManager<ScoreManager>()->AddScore(1000);
                    }
                }

                // Destroy the projectile after collision
                if (!projectile.pObject->IsDestroyed())
                {
                    projectile.pObject->Destroy();
                    projectile.lifespan = 0.0f;
                }
                return; // Stop further checks for this projectile
            }
        }

        // Push all children onto the stack for further checks
        for (auto* child : current->GetChildren())
        {
            if (child)
            {
                stack.push(child);
            }
        }
    }
}


//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------