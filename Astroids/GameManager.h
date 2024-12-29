#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include "box2d/box2d.h"
#include "EnemyAIManager.h"
#include "GameObject.h"
#include "ScoreManager.h"
#include "BaseManager.h"
#include "WindowManager.h"
#include "CollisionListener.h"

class GameManager
{
public:
	GameManager(WindowManager & windowManager);
	~GameManager();

	void EndGame();

	void Update();
	void UpdateGameObjects();

	void Render();

	template <typename T>
	void AddManager();

	template <typename T>
	T * GetManager()
	{
		auto it = mManagers.find(typeid(T));
		if (it != mManagers.end())
		{
			return dynamic_cast<T *>(it->second);
		}
		return nullptr;
	}

	GameObject * CreateNewGameObject(ETeam team, GameObject * pParent);

	GameObject * GetRootGameObject();

	bool IsGameOver() const;

	b2World & GetPhysicsWorld();

	// Window
	WindowManager & mWindowManager;
	sf::RenderWindow * mpWindow;
	sf::Event mEvent;

private:
	void CleanUpDestroyedGameObjects(GameObject * pRoot);

	void RenderImGui();

	void InitWindow();
	
	void GameOver();

	std::vector<std::string> GetCommonResourcePaths();

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

	// Box2d
	b2World mPhysicsWorld;
	CollisionListener mCollisionListener;
};

