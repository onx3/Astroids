#pragma once

#include <SFML/Graphics.hpp>

class Player : public sf::Drawable
{
public:
	Player();
	~Player();

	void SetPosition(const sf::Vector2f & pos);
	void Move(const sf::Vector2f & offset);

protected:
	void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

private:
	sf::Texture mTexture;
	sf::Sprite mSprite;

	float mVelocityX;
	float mVelocityY;
};

