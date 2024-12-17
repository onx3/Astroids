#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "BaseManager.h"

enum class EEnemy
{
	Ship,
	Ufo,
	Asteroid
};

class EnemyAIManager : public BaseManager
{
public:
	EnemyAIManager(GameManager * pGameManager);
	EnemyAIManager(GameManager * pGameManager, int enemyCount);
	~EnemyAIManager();

	void Update() override;

	void RemoveEnemy(GameObject * enemy);
	void RespawnEnemy(EEnemy type, sf::Vector2f pos);

	void AddEnemies(int count, EEnemy type, sf::Vector2f pos);

	const std::vector<GameObject *> & GetEnemies() const;

private:
	std::string GetEnemyFile(EEnemy type);
	void CleanUpDeadEnemies();

	sf::Vector2f GetRandomSpawnPosition();

	const int mMaxEnemies = 10;
	GameManager * mpGameManager;
	std::vector<GameObject *> mEnemyObjects;
};

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------