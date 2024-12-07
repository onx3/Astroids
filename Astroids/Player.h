#pragma once

#include <SFML/Graphics.hpp>

class Player : public sf::Drawable
{
public:
	Player();
	~Player();

	sf::Vector2f GetPosition() const;

	void SetPosition(const sf::Vector2f & pos);

	float GetWidth();
	float GetHeight();

	void Move(const sf::Vector2f & offset);

	void SetRotation(float angle);

	void SetOriginToCenter();

protected:
	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
	sf::Texture mTexture;
	sf::Sprite mSprite;

	float mVelocityX;
	float mVelocityY;
};

