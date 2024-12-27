#include "GameObject.h"
#include <cassert>
#include <imgui.h>
#include "SpriteComponent.h"
#include "BDConfig.h"
#include "GameComponent.h"
#include "GameManager.h"
#include "PlayerManager.h"


GameObject::GameObject(GameManager * pGameManager, ETeam team, GameObject * pParent)
    : mDeltaTime(0.f)
    , mpGameManager(pGameManager)
    , mIsDestroyed(false)
    , mIsActive(true)
    , mTeam(team)
    , mChildGameObjects()
    , mpPhysicsBody(nullptr)
    , mBoundingBox()
{
    mClock.restart();
    if (pParent)
    {
        pParent->AddChild(this);
    }

    auto spriteComp = std::make_shared<SpriteComponent>(this);
    AddComponent(spriteComp);
}

//------------------------------------------------------------------------------------------------------------------------

GameObject::~GameObject()
{
    CleanUpChildren();
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::CleanUpChildren()
{
    for (auto * pChild : mChildGameObjects)
    {
        if (pChild)
        {
            pChild->CleanUpChildren();
            delete pChild;
        }
    }
    mChildGameObjects.clear();
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::Destroy()
{
    mIsDestroyed = true;
}

//------------------------------------------------------------------------------------------------------------------------

bool GameObject::IsDestroyed() const
{
    return mIsDestroyed;
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::CreatePhysicsBody(b2World * world, const sf::Vector2f & size, bool isDynamic)
{
    // Define the body
    b2BodyDef bodyDef;
    bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
    bodyDef.position.Set(GetPosition().x / PIXELS_PER_METER, GetPosition().y / PIXELS_PER_METER);
    bodyDef.bullet = true; // More acurate collision checks
    bodyDef.awake = true;

    // Create the body in the Box2D world
    mpPhysicsBody = world->CreateBody(&bodyDef);

    // Define the shape
    b2PolygonShape boxShape;
    boxShape.SetAsBox((size.x / 2.0f) / PIXELS_PER_METER, (size.y / 2.0f) / PIXELS_PER_METER);

    // Define the fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = isDynamic ? 1.0f : 0.0f;
    fixtureDef.friction = 0.3f;

    // Attach the fixture to the body
    mpPhysicsBody->CreateFixture(&fixtureDef);

    // Set user data for later use
    mpPhysicsBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);

    // Initialize the bounding box
    mBoundingBox.setSize(size);
    mBoundingBox.setOrigin(size.x / 2.0f, size.y / 2.0f); // Center the origin for proper rotation
    mBoundingBox.setFillColor(sf::Color(0, 0, 0, 0)); // Transparent fill
    mBoundingBox.setOutlineColor(sf::Color::Green);   // Green outline for debugging
    mBoundingBox.setOutlineThickness(1.0f);
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::DestroyPhysicsBody(b2World * world)
{
    if (mpPhysicsBody)
    {
        world->DestroyBody(mpPhysicsBody);
        mpPhysicsBody = nullptr;
    }
}

//------------------------------------------------------------------------------------------------------------------------

b2Body * GameObject::GetPhysicsBody() const
{
    return mpPhysicsBody;
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::Update()
{
    if (!mIsDestroyed)
    {
        mDeltaTime = mClock.restart().asSeconds();

        // Update components
        for (auto & component : mComponents)
        {
            component.second->Update();
        }

        // Update child objects
        for (auto * pChild : mChildGameObjects)
        {
            if (pChild)
            {
                pChild->Update();
            }
        }

        // Update bounding box to match physics body
        if (mpPhysicsBody)
        {
            float scale = PIXELS_PER_METER;
            b2Vec2 bodyPos = mpPhysicsBody->GetPosition();
            float bodyAngle = mpPhysicsBody->GetAngle();

            mBoundingBox.setPosition(bodyPos.x * scale, bodyPos.y * scale);
            mBoundingBox.setRotation(bodyAngle * 180.0f / b2_pi); // Convert radians to degrees
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------

float GameObject::GetDeltaTime() const
{
    return mDeltaTime;
}

//------------------------------------------------------------------------------------------------------------------------

ETeam GameObject::GetTeam() const
{
    return mTeam;
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::SetTeam(ETeam team)
{
    mTeam = team;
}

//------------------------------------------------------------------------------------------------------------------------

sf::Vector2f GameObject::GetPosition() const
{
    auto pGameObjectSprite = GetComponent<SpriteComponent>().lock();
    if (pGameObjectSprite)
    {
        return pGameObjectSprite->GetPosition();
    }
    return sf::Vector2f();
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::SetPosition(const sf::Vector2f & position)
{
    auto pGameObjectSprite = GetComponent<SpriteComponent>().lock();
    if (pGameObjectSprite)
    {
        return pGameObjectSprite->SetPosition(position);
    }
}

//------------------------------------------------------------------------------------------------------------------------

float GameObject::GetRotation() const
{
    auto pGameObjectSprite = GetComponent<SpriteComponent>().lock();
    if (pGameObjectSprite)
    {
        return pGameObjectSprite->GetRotation();
    }
    return 0.0f;
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::SetRotation(float angle)
{
    auto pGameObjectSprite = GetComponent<SpriteComponent>().lock();
    if (pGameObjectSprite)
    {
        return pGameObjectSprite->SetRotation(angle);
    }
}

//------------------------------------------------------------------------------------------------------------------------

sf::Vector2f GameObject::GetSize() const
{
    auto pGameObjectSprite = GetComponent<SpriteComponent>().lock();
    if (pGameObjectSprite)
    {
        return pGameObjectSprite->GetSprite().getGlobalBounds().getSize();
    }
    return sf::Vector2f();
}

//------------------------------------------------------------------------------------------------------------------------

GameManager & GameObject::GetGameManager() const
{
    return *mpGameManager;
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::AddChild(GameObject * pChild)
{
    mChildGameObjects.push_back(pChild);
}

//------------------------------------------------------------------------------------------------------------------------

std::vector<GameObject *> & GameObject::GetChildren()
{
    return mChildGameObjects;
}

//------------------------------------------------------------------------------------------------------------------------

std::vector<GameComponent *> GameObject::GetAllComponents()
{
    std::vector<GameComponent *> components;

    // Validate 'this' pointer
    if (!this)
    {
        return components;
    }

    // Validate mComponents
    if (mComponents.empty() && mComponents.size() > 0) // Corruption check
    {
        return components;
    }

    // Safe iteration
    for (const auto & [type, component] : mComponents)
    {
        if (component) // Ensure the shared_ptr is not null
        {
            components.push_back(component.get());
        }
    }

    return components;
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::SetActiveState(bool active)
{
    mIsActive = active;
}

//------------------------------------------------------------------------------------------------------------------------

bool GameObject::IsActive()
{
    return mIsActive;
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::DebugImGuiInfo()
{
#if IMGUI_ENABLED()
    // Show Game Object stuff
    for (auto & component : mComponents)
    {
        auto * pPlayer = GetGameManager().GetManager<PlayerManager>()->GetPlayers()[0];
        if (this == pPlayer)
        {
            ImGui::Text("Children count: %zu", pPlayer->GetChildren().size());
        }
        // Update each component
        if (ImGui::CollapsingHeader(component.first.name()))
        {
            component.second->DebugImGuiComponentInfo();
        }
    }
#endif
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    for (auto & pComponent : mComponents)
    {
        pComponent.second->draw(target, states);
    }

    for (auto * child : mChildGameObjects)
    {
        if (child)
        {
            target.draw(*child, states);
        }
    }

    // Draw the bounding box for debugging
    target.draw(mBoundingBox, states);
}


//------------------------------------------------------------------------------------------------------------------------
//EOF
//------------------------------------------------------------------------------------------------------------------------