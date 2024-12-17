#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include "EnemyAIManager.h"
#include "GameObject.h"
#include "ScoreManager.h"
#include "BaseManager.h"

class GameManager
{
public:
	GameManager();
	~GameManager();

	void EndGame();

	void Update();
	void UpdateGameObjects();

	void Render();

	void CheckCollision();

	template <typename T>
	void AddManager();

	template <typename T>
	T * GetManager();

	GameObject * CreateNewGameObject(ETeam team = ETeam::Neutral);

	std::vector<GameObject *> & GetGameObjects();

	sf::RenderWindow * mpWindow;
	sf::Event mEvent;

private:
	void PollEvents();

	void RenderImGui();

	void InitWindow();

	std::unordered_map<std::type_index, BaseManager *> mManagers;

	sf::Texture mBackgroundTexture;
	sf::Sprite mBackgroundSprite;

	std::vector<GameObject *> mGameObjects;

	sf::Clock mClock;
};

