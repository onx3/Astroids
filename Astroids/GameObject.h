#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include "GameComponent.h" // Include GameComponent header here

class GameObject : public sf::Drawable
{
public:
    GameObject();
    virtual ~GameObject() = default;

    sf::Vector2f GetPosition() const;
    void SetPosition(const sf::Vector2f & position);
    void SetPosition(float x, float y);

    float GetWidth() const;
    float GetHeight() const;

    void Move(const sf::Vector2f & offset);
    void Move(float x, float y);

    void SetRotation(float angle);
    void SetOriginToCenter();

    void AddComponent(std::shared_ptr<GameComponent> component);

    virtual void Update() = 0;

protected:
    void SetTexture(const std::string & file);
    virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

    sf::Texture mTexture;
    sf::Sprite mSprite;
    std::vector<std::shared_ptr<GameComponent>> mComponents;
};
