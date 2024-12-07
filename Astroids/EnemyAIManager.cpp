#include "EnemyAIManager.h"

EnemyAIManager::EnemyAIManager()
	: mEnemies()
{

}

//------------------------------------------------------------------------------------------------------------------------

EnemyAIManager::EnemyAIManager(int enemyCount)
	: mEnemies()
{
	for (int ii = 0; ii < enemyCount; ++ii)
	{
		mEnemies.push_back(new EnemyAI(EEnemy::Ufo));
	}
}

//------------------------------------------------------------------------------------------------------------------------

EnemyAIManager::~EnemyAIManager()
{
	for (auto * enemy : mEnemies)
	{
		delete enemy;
	}
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAIManager::AddEnemies(int count, EEnemy type, sf::Vector2f pos)
{
	for (int ii = 0; ii < count; ++ii)
	{
		mEnemies.push_back(new EnemyAI(type));
		mEnemies.back()->SetPosition(pos);
	}
}

//------------------------------------------------------------------------------------------------------------------------

const std::vector<EnemyAI *> & EnemyAIManager::GetAllEnemies()
{
	return mEnemies;
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------