#include "GameObject.h"
#include <cassert>
#include "SpriteComponent.h"

GameObject::GameObject()
{
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
    for (auto & component : mComponents)
    {
        component.second->Update(); // Update each component
    }
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    auto spriteComponent = GetComponent<SpriteComponent>().lock();
    if (spriteComponent)
    {
        target.draw(spriteComponent->GetSprite(), states);
    }
}

//------------------------------------------------------------------------------------------------------------------------
//EOF
