#pragma once

#include <SFML/Graphics.hpp>

class GameManager;
class ScoreManager
{
public:
	ScoreManager(GameManager * pGameManager);
	void AddScore(int points);

	const sf::Text & GetScoreText();

	std::vector<sf::Sprite> & GetSpriteLives();

private:
	int mScore;
	sf::Font mFont;
	sf::Text mScoreText;

	sf::Texture mLifeTexture;
	sf::Sprite mLifeSprite;
	std::vector<sf::Sprite> mSpriteLives;

	GameManager * mpGameManager;
};

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------