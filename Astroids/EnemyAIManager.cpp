#include "EnemyAIManager.h"
#include "GameObject.h"
#include <memory>
#include <cassert>
#include "SpriteComponent.h"

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
		GameObject * gameObj = new GameObject();
		auto spriteComp = std::make_shared<SpriteComponent>();

		std::string file = "Art/EnemyShip.png";

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

void EnemyAIManager::AddEnemies(int count, EEnemy type, sf::Vector2f pos)
{
	for (int ii = 0; ii < count; ++ii)
	{
		GameObject * gameObj = new GameObject();
		auto spriteComp = std::make_shared<SpriteComponent>();

		std::string file;
		switch (type)
		{
			case (EEnemy::Ship):
			{
				file = "Art/EnemyShip.png";
				break;
			}
			case (EEnemy::Ufo):
			{
				file = "Art/EnemyUFO.png";
				break;
			}
		}

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
// EOF
//------------------------------------------------------------------------------------------------------------------------