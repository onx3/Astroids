#include "AstroidsPrivate.h"
#include <iostream>
#include <cstdlib>

int main()
{
    WindowManager windowManager;

    while (windowManager.GetWindow()->isOpen())
    {
        GameManager * pGameManager = new GameManager(windowManager);

        while (windowManager.GetWindow()->isOpen() && !pGameManager->IsGameOver())
        {
            windowManager.PollEvents();
            pGameManager->Update();
            pGameManager->Render();
        }
        delete pGameManager;

        bool waitingForRestart = true;
        while (windowManager.GetWindow()->isOpen() && waitingForRestart)
        {
            windowManager.PollEvents();

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                waitingForRestart = false;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                windowManager.GetWindow()->close();
            }
        }
    }

    return 0;
}
