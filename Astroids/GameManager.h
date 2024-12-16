#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "EnemyAIManager.h"
#include "GameObject.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	void Update();
	void UpdateGameObjects();

	void Render();

	EnemyAIManager & GetEnemyAiManager();

	std::vector<GameObject *> & GetGameObjects();

	sf::RenderWindow * mpWindow;
	sf::Event mEvent;

private:
	void PollEvents();

	void RenderImGui();

	void InitEnemies();
	void InitPlayer();
	void InitWindow();

	EnemyAIManager mEnemyManager;

	sf::Texture mBackgroundTexture;
	sf::Sprite mBackgroundSprite;

	std::vector<GameObject *> mGameObjects;

	sf::Clock mClock;
};

