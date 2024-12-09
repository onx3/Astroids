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
	EnemyAIManager();
	EnemyAIManager(int enemyCount);
	~EnemyAIManager();

	void AddEnemies(int count, EEnemy type, sf::Vector2f pos);

	const std::vector<GameObject *> & GetAllEnemies();

private:

	std::vector<GameObject *> mEnemies;
};