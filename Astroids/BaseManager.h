#pragma once
#include "AstroidsPrivate.h"

class GameManager;

class BaseManager
{
public:
	explicit BaseManager(GameManager * pGameManager);
	~BaseManager();

	virtual void Update();

	virtual void OnGameEnd();

protected:
	GameManager * mpGameManager;
};

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------