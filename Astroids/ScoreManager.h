#pragma once

#include "AstroidsPrivate.h"
#include "BaseManager.h"

class GameManager;
class ScoreManager : public BaseManager
{
public:
	ScoreManager(GameManager * pGameManager);
	void AddScore(int points);

	const sf::Text & GetScoreText();

	std::vector<sf::Sprite> & GetSpriteLives();
	int GetScore();

private:
	int mScore;
	sf::Font mFont;
	sf::Text mScoreText;

	sf::Texture mLifeTexture;
	sf::Sprite mLifeSprite;
	std::vector<sf::Sprite> mSpriteLives;
};

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------