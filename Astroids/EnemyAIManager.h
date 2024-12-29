#pragma once
#include "AstroidsPrivate.h"
#include "BaseManager.h"
#include <vector>
#include "GameObject.h"
#include "DropManager.h"

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

	void OnGameEnd() override;

	void RemoveEnemy(GameObject * pEnemy);
	void RespawnEnemy(EEnemy type, sf::Vector2f pos);

	void AddEnemies(int count, EEnemy type, sf::Vector2f pos);
	void DestroyAllEnemies();

	const std::vector<GameObject *> & GetEnemies() const;

	void OnDeath(GameObject * pEnemy);

private:
	std::string GetEnemyFile(EEnemy type);
	void CleanUpDeadEnemies();

	sf::Vector2f GetRandomSpawnPosition();

	EDropType DetermineDropType() const;

	const int mMaxEnemies = 25;
	std::vector<GameObject *> mEnemyObjects;
};

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------