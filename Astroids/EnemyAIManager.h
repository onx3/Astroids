#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "GameObject.h"

enum class EEnemy
{
	Ship,
	Ufo
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

	const std::vector<GameObject *> & GetAllEnemies();

private:
	std::string GetEnemyFile(EEnemy type);
	void CleanUpDeadEnemies();

	std::vector<GameObject *> mEnemies;
	const int mMaxEnemies = 3;
	GameManager * mpGameManager;
};