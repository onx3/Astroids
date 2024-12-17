#pragma once

class GameManager;

class BaseManager
{
public:
	explicit BaseManager(GameManager * pGameManager);
	~BaseManager();

	virtual void Update();

protected:
	GameManager * mpGameManager;
};

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------