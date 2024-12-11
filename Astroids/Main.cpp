#include <iostream>
#include "GameManager.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>

int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    GameManager * pGameManager = new GameManager();
    auto * pWindow = pGameManager->mpWindow;
    auto & event = pGameManager->mEvent;

    while (pWindow->isOpen())
    {
        pGameManager->Update();

        // Render the game
        pGameManager->Render();
    }

    delete pGameManager;
    return 0;
}
