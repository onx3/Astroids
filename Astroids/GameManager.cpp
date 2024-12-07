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

#if 0
    auto relativeMousePos = sf::Mouse::getPosition(*mpWindow);
    sf::Vector2f spriteSize = sf::Vector2f(mPlayer.GetWidth(), mPlayer.GetHeight()); 

    mPlayer.SetPosition(sf::Vector2f(
        float(relativeMousePos.x) - spriteSize.x / 2.0f,
        float(relativeMousePos.y) - spriteSize.y / 2.0f
    ));
#endif
    // Get the mouse position relative to the window
    auto relativeMousePos = sf::Mouse::getPosition(*mpWindow);

    // Convert to world coordinates (if you're using a view, this adjusts for it)
    sf::Vector2f worldMousePos = mpWindow->mapPixelToCoords(relativeMousePos);

    // Get the player's position
    sf::Vector2f playerPos = mPlayer.GetPosition(); // Assuming GetPosition() gets the sprite's center

    // Calculate the difference
    sf::Vector2f direction = worldMousePos - playerPos;

    // Calculate the angle in radians
    float angle = atan2(direction.y, direction.x);

    // Convert to degrees (SFML uses degrees for rotation)
    float angleInDegrees = angle * 180.0f / 3.14159f;

    // Set the player's rotation
    mPlayer.SetRotation(angleInDegrees);

    // Center the player in the window
    sf::Vector2u windowSize = mpWindow->getSize();
    sf::Vector2f centerPosition(float(windowSize.x) / 2.0f, float(windowSize.y) / 2.0f);
    mPlayer.SetPosition(centerPosition);
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