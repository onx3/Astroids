#include "GameManager.h"
#include <cassert>
#include <imgui.h>
#include <imgui-SFML.h>
#include "SpriteComponent.h"
#include "ControlledMovementComponent.h"
#include "ProjectileComponent.h"
#include "BDConfig.h"

GameManager::GameManager()
    : mpWindow(nullptr)
    , mEvent()
    , mBackgroundTexture()
    , mBackgroundSprite()
    , mPlayer(this)
    , mEnemyManager(this)
{
    mClock.restart();
    InitWindow();
    InitPlayer();
    InitEnemies();
}

//------------------------------------------------------------------------------------------------------------------------

GameManager::~GameManager()
{
    delete mpWindow;
    ImGui::SFML::Shutdown();
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::PollEvents()
{
    ImGui::SFML::ProcessEvent(mEvent);
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
    mEnemyManager.UpdateEnemies();
    
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::RenderImGui()
{
#if IMGUI_ENABLED()
    static bool showWindow = false;
    ImGui::SFML::Update(*mpWindow, mClock.restart());

   
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
    {
        showWindow = true;
    }

    if (showWindow)
    {
        ImGui::Begin("Player Components", &showWindow);

        mPlayer.DebugImGuiInfo();
        ImGui::End();
    }

    ImGui::SFML::Render(*mpWindow);
#endif

}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::Render()
{
    // Order Matters

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
            mpWindow->draw(*pEnemy);
        }
    }

    RenderImGui();

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
    // Sprite Component
    {
        sf::Vector2u windowSize = mpWindow->getSize();
        sf::Vector2f centerPosition(float(windowSize.x) / 2.0f, float(windowSize.y) / 2.0f);

        auto pSpriteComponent = mPlayer.GetComponent<SpriteComponent>().lock();

        if (pSpriteComponent)
        {
            std::string file = "Art/player.png";
            pSpriteComponent->SetSprite(file);
            pSpriteComponent->SetPosition(centerPosition);
        }
    }

    // Controlled Movement Component
    {
        auto pMovementComponent = mPlayer.GetComponent<ControlledMovementComponent>().lock();
        if (!pMovementComponent)
        {
            auto pMovementComponent = std::make_shared<ControlledMovementComponent>(&mPlayer);
            mPlayer.AddComponent(pMovementComponent);
        }
    }

    // Projectile Component
    {
        auto pProjectileComponent = mPlayer.GetComponent<ProjectileComponent>().lock();
        if (!pProjectileComponent)
        {
            mPlayer.AddComponent(std::make_shared<ProjectileComponent>(&mPlayer));
        }
    }
    
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::InitWindow()
{
    mpWindow = new sf::RenderWindow(sf::VideoMode(1000, 1000), "Game", sf::Style::Default);
    mpWindow->setFramerateLimit(240);

    std::string file = "Art/Background/background2.png";
    assert(mBackgroundTexture.loadFromFile(file));
    mBackgroundSprite.setTexture(mBackgroundTexture);

    // Scale to window size
    mBackgroundSprite.setScale(float(mpWindow->getSize().x) / mBackgroundTexture.getSize().x, float(mpWindow->getSize().y) / mBackgroundTexture.getSize().y);

    ImGui::SFML::Init(*mpWindow);
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------