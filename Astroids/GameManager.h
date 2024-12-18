#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "EnemyAIManager.h"
#include "GameObject.h"
#include "ScoreManager.h"
#include "BaseManager.h"
#include "WindowManager.h"

class GameManager
{
public:
	GameManager(WindowManager & windowManager);
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

	bool IsGameOver() const;

	// Window
	WindowManager & mWindowManager;
	sf::RenderWindow * mpWindow;
	sf::Event mEvent;

private:
	void CleanUpDestroyedGameObjects(GameObject * pRoot);

	void RenderImGui();

	void InitWindow();
	
	void GameOver();

	bool mShowImGuiWindow;
	std::unordered_map<std::type_index, BaseManager *> mManagers;
	sf::Texture mBackgroundTexture;
	sf::Sprite mBackgroundSprite;
	sf::Texture mCursorTexture;
	sf::Sprite mCursorSprite;
	GameObject * mpRootGameObject;
	sf::Clock mClock;

	// Audio
	sf::SoundBuffer mSoundBuffer;
	sf::Sound mSound;
	bool mSoundPlayed;

	// GameOver
	bool mIsGameOver;
	sf::Text mGameOverText;
	sf::Text mScoreText;
	sf::Font mFont;
};

