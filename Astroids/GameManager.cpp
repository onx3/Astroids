#include "GameManager.h"
#include "SpriteComponent.h"
#include "cassert"

GameManager::GameManager()
    : mpWindow(nullptr)
    , mEvent()
    , mBackgroundTexture()
    , mBackgroundSprite()
    , mPlayer()
{
    InitWindow();
    InitPlayer();
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

#if 0
    auto relativeMousePos = sf::Mouse::getPosition(*mpWindow);
    sf::Vector2f spriteSize = sf::Vector2f(mPlayer.GetWidth(), mPlayer.GetHeight()); 

    mPlayer.SetPosition(sf::Vector2f(
        float(relativeMousePos.x) - spriteSize.x / 2.0f,
        float(relativeMousePos.y) - spriteSize.y / 2.0f
    ));
#endif

    mPlayer.Update();
    
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
        for (auto * pEnemy : enemies)
        {
            //enemy.SetTexture();
            mpWindow->draw(*pEnemy);
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

void GameManager::InitPlayer()
{
    sf::Vector2u windowSize = mpWindow->getSize();
    sf::Vector2f centerPosition(float(windowSize.x) / 2.0f, float(windowSize.y) / 2.0f);

    auto spriteComponents = mPlayer.GetComponents<SpriteComponent>();
    assert(spriteComponents.size() == 1);

    std::string file = "Art/player.png";
    spriteComponents[0]->SetSprite(file);
    spriteComponents[0]->SetPosition(centerPosition);
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::InitWindow()
{
    mpWindow = new sf::RenderWindow(sf::VideoMode(1000, 1000), "Game", sf::Style::Default);
    mpWindow->setFramerateLimit(240);

    std::string file = "Art/Background/background2.png";
    if (!mBackgroundTexture.loadFromFile(file))
    {
        // Handle error
        std::cout << "Can't load the file : " << file;
    }
    mBackgroundSprite.setTexture(mBackgroundTexture);

    // Scale to window size
    mBackgroundSprite.setScale(float(mpWindow->getSize().x) / mBackgroundTexture.getSize().x, float(mpWindow->getSize().y) / mBackgroundTexture.getSize().y);

    //mpWindow->setMouseCursorVisible(false);
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------