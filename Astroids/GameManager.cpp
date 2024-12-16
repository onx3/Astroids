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
    , mEnemyManager(this)
    , mGameObjects()
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

    UpdateGameObjects();
    mEnemyManager.UpdateEnemies();
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::UpdateGameObjects()
{
    for (int i = 0; i < mGameObjects.size();)
    {
        if (mGameObjects[i]->IsDestroyed())
        {
            delete mGameObjects[i];                    // Clean up the destroyed object
            mGameObjects.erase(mGameObjects.begin() + i); // Erase and stay at the same index
        }
        else
        {
            mGameObjects[i]->Update(); // Update active objects
            ++i;                       // Only increment if no erase was done
        }
    }
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
        ImGui::Begin("Game Objects", &showWindow);

        for (size_t i = 0; i < mGameObjects.size(); ++i)
        {
            auto * pGameObj = mGameObjects[i];

            // Create a collapsible node for each GameObject
            std::string headerLabel = "GameObject " + std::to_string(i);
            if (ImGui::TreeNode(headerLabel.c_str()))
            {
                // Inside the drop-down, display the GameObject's debug info
                pGameObj->DebugImGuiInfo();

                ImGui::TreePop(); // End the collapsible node
            }
        }
        ImGui::End();
    }

    ImGui::SFML::Render(*mpWindow);
#endif

}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::Render()
{
    mpWindow->clear();

    // Draw Background
    mpWindow->draw(mBackgroundSprite);

    // Draw All GameObjects
    for (auto * pGameObj : mGameObjects)
    {
        mpWindow->draw(*pGameObj);
    }

    RenderImGui();

    mpWindow->display(); // Renderer is done keep at the end.
}

//------------------------------------------------------------------------------------------------------------------------

EnemyAIManager & GameManager::GetEnemyAiManager()
{
    return mEnemyManager;
}

//------------------------------------------------------------------------------------------------------------------------

std::vector<GameObject *> & GameManager::GetGameObjects()
{
    return mGameObjects;
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::InitEnemies()
{
    mEnemyManager.AddEnemies(1, EEnemy::Asteroid, sf::Vector2f(100.f, 100.f));
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::InitPlayer()
{
    auto * pPlayer = new GameObject(this, ETeam::Friendly);
    mGameObjects.push_back(pPlayer);

    // Sprite Component
    {
        sf::Vector2u windowSize = mpWindow->getSize();
        sf::Vector2f centerPosition(float(windowSize.x) / 2.0f, float(windowSize.y) / 2.0f);

        auto pSpriteComponent = pPlayer->GetComponent<SpriteComponent>().lock();

        if (pSpriteComponent)
        {
            std::string file = "Art/player.png";
            auto scale = sf::Vector2f(.5f, .5f);
            pSpriteComponent->SetSprite(file, scale);
            pSpriteComponent->SetPosition(centerPosition);
            pSpriteComponent->SetOriginToCenter();
        }
    }

    // Controlled Movement Component
    {
        auto pMovementComponent = pPlayer->GetComponent<ControlledMovementComponent>().lock();
        if (!pMovementComponent)
        {
            auto pMovementComponent = std::make_shared<ControlledMovementComponent>(pPlayer);
            pPlayer->AddComponent(pMovementComponent);
        }
    }

    // Projectile Component
    {
        auto pProjectileComponent = pPlayer->GetComponent<ProjectileComponent>().lock();
        if (!pProjectileComponent)
        {
            pPlayer->AddComponent(std::make_shared<ProjectileComponent>(pPlayer));
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