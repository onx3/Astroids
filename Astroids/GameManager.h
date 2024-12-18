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

	void CheckCollisionRecursive(GameObject * root, GameObject * pPlayer);

	template <typename T>
	void AddManager();

	template <typename T>
	T * GetManager();

	GameObject * CreateNewGameObject(ETeam team, GameObject * pParent);

	GameObject * GetRootGameObject();
	sf::RenderWindow * mpWindow;
	sf::Event mEvent;

private:
	void CleanUpDestroyedGameObjects(GameObject * pRoot);

	void PollEvents();

	void RenderImGui();

	void InitWindow();

	bool mShowImGuiWindow;

	std::unordered_map<std::type_index, BaseManager *> mManagers;

	sf::Texture mBackgroundTexture;
	sf::Sprite mBackgroundSprite;

	sf::Texture mCursorTexture;
	sf::Sprite mCursorSprite;

	GameObject * mpRootGameObject;

	sf::Clock mClock;
};

