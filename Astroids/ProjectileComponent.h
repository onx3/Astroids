#pragma once
#include "AstroidsPrivate.h"
#include "GameComponent.h"
#include <SFML/System/Vector2.hpp>
#include <vector>
#include <string>

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

    void Update(float deltaTime) override;

    void draw(sf::RenderTarget & target, sf::RenderStates states) override;

    void DebugImGuiComponentInfo() override;

private:
    struct Projectile
    {
        GameObject * pObject;
        float lifespan;
        int damage;
        sf::Vector2f direction;
    };

    void UpdateProjectiles(float deltaTime);

    std::vector<Projectile> mProjectiles;
    float mSpeed;
    float mCooldown;
    float mTimeSinceLastShot;
    EProjectile mLastUsedProjectile;
    std::string mName;
};
