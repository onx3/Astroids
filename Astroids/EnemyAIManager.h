#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "GameObject.h"

enum class EEnemy
{
	Ship,
	Ufo,
	Asteroid
};

class EnemyAIManager
{
public:
	EnemyAIManager(GameManager * pGameManager);
	EnemyAIManager(GameManager * pGameManager, int enemyCount);
	~EnemyAIManager();

	void UpdateEnemies();

	void RemoveEnemy(GameObject * enemy);
	void RespawnEnemy(EEnemy type, sf::Vector2f pos);

	void AddEnemies(int count, EEnemy type, sf::Vector2f pos);

private:
	std::string GetEnemyFile(EEnemy type);
	void CleanUpDeadEnemies();

	const int mMaxEnemies = 10;
	GameManager * mpGameManager;
};

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------