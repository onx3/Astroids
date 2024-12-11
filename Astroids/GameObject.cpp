#include "GameObject.h"
#include <cassert>
#include "SpriteComponent.h"

GameObject::GameObject()
    : mDeltaTime(0.f)
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

void GameObject::Update()
{
    mDeltaTime = mClock.restart().asSeconds();
    for (auto & component : mComponents)
    {
        component.second->Update(); // Update each component
    }
}

//------------------------------------------------------------------------------------------------------------------------

float GameObject::GetDeltaTime() const
{
    return mDeltaTime;
}

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

void GameObject::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    for (auto & pComponent : mComponents)
    {
        pComponent.second->draw(target, states);
    }
}

//------------------------------------------------------------------------------------------------------------------------
//EOF
