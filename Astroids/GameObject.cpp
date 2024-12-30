#include "AstroidsPrivate.h"
#include "GameObject.h"
#include <cassert>
#include <imgui.h>
#include "SpriteComponent.h"
#include "BDConfig.h"
#include "GameComponent.h"
#include "PlayerManager.h"


GameObject::GameObject(GameManager * pGameManager, ETeam team, GameObject * pParent)
    : mpGameManager(pGameManager)
    , mIsDestroyed(false)
    , mIsActive(true)
    , mTeam(team)
    , mChildGameObjects()
    , mpParent(pParent)
    , mpPhysicsBody(nullptr)
{
    if (pParent)
    {
        pParent->AddChild(this);
    }

    auto spriteComp = std::make_shared<SpriteComponent>(this);
    AddComponent(spriteComp);
    spriteComp->SetOriginToCenter();
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
    for (auto * pChild : mChildGameObjects)
    {
        if (pChild)
        {
            pChild->Destroy();
        }
    }
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

void GameObject::NotifyParentOfDeletion()
{
    if (mpParent)
    {
        auto & siblings = mpParent->GetChildren();
        auto it = std::find(siblings.begin(), siblings.end(), this);
        if (it != siblings.end())
        {
            siblings.erase(it);
        }
    }
}


//------------------------------------------------------------------------------------------------------------------------

void GameObject::Update(float deltaTime)
{
    if (!mIsDestroyed)
    {
        // Update components
        for (auto & component : mComponents)
        {
            component.second->Update(deltaTime);
        }

        // Update child objects
        for (auto * pChild : mChildGameObjects)
        {
            if (pChild && !pChild->IsDestroyed())
            {
                pChild->Update(deltaTime);
            }
        }
    }
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
    if (pChild)
    {
        mChildGameObjects.push_back(pChild);
        pChild->SetParent(this);
    }
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::RemoveChild(GameObject * child)
{
    auto it = std::find(mChildGameObjects.begin(), mChildGameObjects.end(), child);
    if (it != mChildGameObjects.end())
    {
        (*it)->SetParent(nullptr);
        mChildGameObjects.erase(it);
    }
}

//------------------------------------------------------------------------------------------------------------------------

std::vector<GameObject *> & GameObject::GetChildren()
{
    return mChildGameObjects;
}

//------------------------------------------------------------------------------------------------------------------------

GameObject * GameObject::GetParent() const
{
    return mpParent;
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::SetParent(GameObject * pParent)
{
    mpParent = pParent;
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
}

//------------------------------------------------------------------------------------------------------------------------
//EOF
//------------------------------------------------------------------------------------------------------------------------