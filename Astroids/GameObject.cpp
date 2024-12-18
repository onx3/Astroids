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
    , mTeam(team)
    , mChildGameObjects()
{
    mClock.restart();
    if (pParent)
    {
        pParent->AddChild(this);
    }

    auto spriteComp = std::make_shared<SpriteComponent>(this);
    AddComponent(spriteComp); // Add a single instance of SpriteComponent
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