#include "GameObject.h"
#include "cassert"
#include "SpriteComponent.h"

GameObject::GameObject()
{
    auto spriteComp = std::make_shared<SpriteComponent>();
    AddComponent(spriteComp);
}

//------------------------------------------------------------------------------------------------------------------------

GameObject::~GameObject()
{
}

//------------------------------------------------------------------------------------------------------------------------

template <typename T>
void GameObject::AddComponent(std::shared_ptr<T> component)
{
    static_assert(std::is_base_of<GameComponent, T>::value, "T must derive from GameComponent");
    mComponents[typeid(T)].push_back(component);
}

//------------------------------------------------------------------------------------------------------------------------

template <typename T>
std::vector<std::shared_ptr<T>> GameObject::GetComponents() const
{
    std::vector<std::shared_ptr<T>> components;
    auto it = mComponents.find(typeid(T));
    if (it != mComponents.end())
    {
        for (const auto & component : it->second)
        {
            components.push_back(std::static_pointer_cast<T>(component));
        }
    }
    return components;
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::Update()
{
    for (auto & pair : mComponents)
    {
        auto & components = pair.second;
        for (auto & component : components)
        {
            component->Update();
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    auto spriteComponents = GetComponents<SpriteComponent>();
    assert(spriteComponents.size() == 1);

    for (const auto & sprite : spriteComponents)
    {
        target.draw(sprite->GetSprite(), states);
    }
}

//------------------------------------------------------------------------------------------------------------------------
//EOF
//------------------------------------------------------------------------------------------------------------------------