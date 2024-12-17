#pragma once
#include "BaseManager.h"
#include "GameManager.h"

class PlayerManager : public BaseManager
{
public:
	PlayerManager(GameManager * pGameManager);
	~PlayerManager();

	void InitPlayer();

	void Update() override;

	const std::vector<GameObject *> & GetPlayers() const;
private:

	void CleanUpDeadPlayers();
	std::vector<GameObject *> mPlayerObjects;
};

