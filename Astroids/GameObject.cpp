#include "GameObject.h"
#include <cassert>
#include <imgui.h>
#include "SpriteComponent.h"
#include "BDConfig.h"
#include "GameComponent.h"
#include "GameManager.h"


GameObject::GameObject(GameManager * pGameManager, ETeam team)
    : mDeltaTime(0.f)
    , mpGameManager(pGameManager)
    , mIsDestroyed(false)
    , mTeam(team)
{
    mClock.restart();
    auto spriteComp = std::make_shared<SpriteComponent>(this);
    AddComponent(spriteComp); // Add a single instance of SpriteComponent
}

//------------------------------------------------------------------------------------------------------------------------

GameObject::~GameObject()
{
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
        for (auto & component : mComponents)
        {
            if (!mIsDestroyed)
            {
                component.second->Update(); // Update each component
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

void GameObject::DebugImGuiInfo()
{
#if IMGUI_ENABLED()
    // Show Game Object stuff
    for (auto & component : mComponents)
    {
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
}

//------------------------------------------------------------------------------------------------------------------------
//EOF
//------------------------------------------------------------------------------------------------------------------------