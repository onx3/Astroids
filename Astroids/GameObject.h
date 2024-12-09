#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <string>
#include "GameComponent.h" // Include GameComponent header here

class GameObject : public sf::Drawable
{
public:
    GameObject();
    ~GameObject();

    template <typename T>
    void AddComponent(std::shared_ptr<T> component);
    template <typename T>
    std::vector<std::shared_ptr<T>> GetComponents() const;

    void Update();

protected:
    virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

    std::unordered_map<std::type_index, std::vector<std::shared_ptr<GameComponent>>> mComponents;
};
