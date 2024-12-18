#include <iostream>
#include "GameManager.h"
#include <cstdlib>

int main()
{
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
