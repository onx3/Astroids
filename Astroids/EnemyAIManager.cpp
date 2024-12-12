#include "EnemyAIManager.h"
#include "GameObject.h"
#include <memory>
#include <cassert>
#include "SpriteComponent.h"

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
	for (int ii = 0; ii < enemyCount; ++ii)
	{
		GameObject * gameObj = new GameObject(mpGameManager);
		auto spriteComp = std::make_shared<SpriteComponent>(gameObj);

		std::string file = GetEnemyFile(EEnemy::Ship);

		spriteComp->SetSprite(file);
		gameObj->AddComponent(spriteComp);
		
		spriteComp->SetPosition(sf::Vector2f(100, 100));
		mEnemies.push_back(gameObj);
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
	GameObject * pEnemy = new GameObject(mpGameManager);
	auto pSpriteComponent = pEnemy->GetComponent<SpriteComponent>().lock();
	if (pSpriteComponent)
	{
		pSpriteComponent->SetSprite(GetEnemyFile(type));
	}

	mEnemies.push_back(pEnemy);
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAIManager::AddEnemies(int count, EEnemy type, sf::Vector2f pos)
{
	for (int ii = 0; ii < count; ++ii)
	{
		GameObject * gameObj = new GameObject(mpGameManager);
		auto spriteComp = std::make_shared<SpriteComponent>(gameObj);

		std::string file = GetEnemyFile(type);

		spriteComp->SetSprite(file);
		gameObj->AddComponent(spriteComp);

		spriteComp->SetPosition(pos);
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