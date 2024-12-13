#include "EnemyAIManager.h"
#include "GameObject.h"
#include <memory>
#include <cassert>
#include "SpriteComponent.h"
#include "RandomMovementComponent.h"

EnemyAIManager::EnemyAIManager(GameManager * pGameManager)
	: mEnemies()
	, mpGameManager(pGameManager)
{

}

//------------------------------------------------------------------------------------------------------------------------

EnemyAIManager::EnemyAIManager(GameManager * pGameManager, int enemyCount)
	: mEnemies()
	, mpGameManager(pGameManager)
{
	AddEnemies(enemyCount, EEnemy::Ship, sf::Vector2f());
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

void EnemyAIManager::UpdateEnemies()
{
	for (auto * pEnemy : mEnemies)
	{
		pEnemy->Update();
	}

	CleanUpDeadEnemies();

	while (mEnemies.size() < mMaxEnemies)
	{
		RespawnEnemy(EEnemy::Ship, sf::Vector2f(100.f, 0.f));
	}
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAIManager::RemoveEnemy(GameObject * enemy)
{
	auto it = std::find(mEnemies.begin(), mEnemies.end(), enemy);
	if (it != mEnemies.end())
	{
		delete * it;
		mEnemies.erase(it);
	}
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAIManager::RespawnEnemy(EEnemy type, sf::Vector2f pos)
{
	AddEnemies(1, type, pos);
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAIManager::AddEnemies(int count, EEnemy type, sf::Vector2f pos)
{
	for (int ii = 0; ii < count; ++ii)
	{
		GameObject * gameObj = new GameObject(mpGameManager);
		auto pSpriteComp = gameObj->GetComponent<SpriteComponent>().lock();

		if (pSpriteComp)
		{
			std::string file = GetEnemyFile(type);

			pSpriteComp->SetSprite(file);
			gameObj->AddComponent(pSpriteComp);

			pSpriteComp->SetPosition(pos);

			auto pRandomMovementComp = std::make_shared<RandomMovementComponent>(gameObj);
			gameObj->AddComponent(pRandomMovementComp);
		}
		mEnemies.push_back(gameObj);
	}
}

//------------------------------------------------------------------------------------------------------------------------

const std::vector<GameObject *> & EnemyAIManager::GetAllEnemies()
{
	return mEnemies;
}

//------------------------------------------------------------------------------------------------------------------------

std::string EnemyAIManager::GetEnemyFile(EEnemy type)
{
	switch (type)
	{
		case (EEnemy::Ship):
		{
			return "Art/EnemyShip.png";
		}
		case (EEnemy::Ufo):
		{
			return "Art/EnemyUFO.png";
		}
		default:
		{
			return "meteorBig.png";
		}
	}
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAIManager::CleanUpDeadEnemies()
{

}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------