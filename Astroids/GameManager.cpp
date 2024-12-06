#include "GameManager.h"
#include "EnemyAI.h"

GameManager::GameManager()
    : mpWindow(nullptr)
    , mEvent()
    , mBackgroundTexture()
    , mBackgroundSprite()
    , mPlayer()
{
    InitWindow();
    InitEnemies();
}

//------------------------------------------------------------------------------------------------------------------------

GameManager::~GameManager()
{
    delete mpWindow;
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::PollEvents()
{
    while (mpWindow->pollEvent(mEvent))
    {
        switch (mEvent.type)
        {
            case sf::Event::Closed:
            {
                mpWindow->close();
                break;
            }
            case sf::Event::KeyPressed:
            {
                if (mEvent.key.code == sf::Keyboard::Escape)
                {
                    mpWindow->close();
                }
                break;
            }
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::Update()
{
    PollEvents();

    auto relativeMousePos = sf::Mouse::getPosition(*mpWindow);
    mPlayer.SetPosition(sf::Vector2f(float(relativeMousePos.x), float(relativeMousePos.y)));
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::Render()
{
    mpWindow->clear();

    // Draw the Game
    {
        // Draw Background
        mpWindow->draw(mBackgroundSprite);

        // Draw Player
        mpWindow->draw(mPlayer);

        // Draw Enemies
        auto & enemies = mEnemyManager.GetAllEnemies();
        for (auto enemy : enemies)
        {
            enemy.SetTexture();
            mpWindow->draw(enemy);
        }
    }

    mpWindow->display(); // Renderer is done keep at the end.
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::InitEnemies()
{
    mEnemyManager.AddEnemies(1, EEnemy::Ship, sf::Vector2f(100.f, 100.f));
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::InitWindow()
{
    mpWindow = new sf::RenderWindow(sf::VideoMode(1000, 1000), "Game", sf::Style::Default);
    mpWindow->setFramerateLimit(240);

    std::string file = "Art/Background/starBackground.png";
    if (!mBackgroundTexture.loadFromFile(file))
    {
        // Handle error
        std::cout << "Can't load the file : " << file;
    }
    mBackgroundSprite.setTexture(mBackgroundTexture);
    // Scale to window size
    mBackgroundSprite.setScale(float(mpWindow->getSize().x) / mBackgroundTexture.getSize().x, float(mpWindow->getSize().y) / mBackgroundTexture.getSize().y);
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------