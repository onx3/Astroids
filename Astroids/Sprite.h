#pragma once

#include <SFML/Graphics.hpp>
#include "GameComponent.h"

class Sprite : public GameComponent
{
public:
	Sprite();
	~Sprite();

	void SetSprite(const std::string & file);
	sf::Sprite GetSprite();

private:
	sf::Texture mTexture;
	sf::Sprite mSprite;
};

