#pragma once

#include <SFML/Graphics.hpp>
#include <cassert>

class Entity : public sf::Drawable
{
public:
	Entity();
	virtual ~Entity() = default;

	sf::Vector2f GetPosition() const;
	void SetPosition(const sf::Vector2f & position);
	void SetPosition(float x, float y);

	float GetWidth() const;
	float GetHeight() const;

	void Move(const sf::Vector2f & offset);
	void Move(float x, float y);

	void SetRotation(float angle);
	void SetOriginToCenter();

	virtual void Update() = 0;

protected:
	void SetTexture(const std::string & file);
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

	sf::Texture mTexture;
	sf::Sprite mSprite;
};

