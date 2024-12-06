#pragma once
#include <vector>
#include "EnemyAI.h"

class EnemyAIManager
{
public:
	EnemyAIManager();
	EnemyAIManager(int enemyCount);
	~EnemyAIManager();

	void AddEnemies(int count, EEnemy type, sf::Vector2f pos);

	const std::vector<EnemyAI> & GetAllEnemies();

private:

	std::vector<EnemyAI> mEnemies;
};