#pragma once
#include "BaseManager.h"
#include "GameObject.h"

enum class EDropType
{
	None,
	NukePickup,
	LifePickup
};

class DropManager : public BaseManager
{
public:
	DropManager(GameManager * pGameManager);

	void Update() override;

	void OnGameEnd() override;

	void CleanUpDrops();

	void SpawnDrop(EDropType dropType, const sf::Vector2f & position);

private:
	std::vector<GameObject *> mDropObjects;
};

