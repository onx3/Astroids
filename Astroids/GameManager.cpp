#include "GameManager.h"
#include <cassert>
#include <imgui.h>
#include <imgui-SFML.h>
#include "SpriteComponent.h"
#include "ControlledMovementComponent.h"
#include "ProjectileComponent.h"
#include "BDConfig.h"
#include "CollisionComponent.h"
#include "HealthComponent.h"
#include "PlayerManager.h"

GameManager::GameManager()
    : mpWindow(nullptr)
    , mEvent()
    , mBackgroundTexture()
    , mBackgroundSprite()
    , mGameObjects()
    , mManagers()
{
    mClock.restart();
    InitWindow();

    AddManager<PlayerManager>();
    AddManager<EnemyAIManager>();
    AddManager<ScoreManager>();
}

//------------------------------------------------------------------------------------------------------------------------

GameManager::~GameManager()
{
    for (auto & obj : mGameObjects)
    {
        delete obj; // Delete each GameObject
    }

    for (auto & manager : mManagers)
    {
        delete manager.second; // Delete each Manager
    }

    delete mpWindow;
    ImGui::SFML::Shutdown();
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::EndGame()
{
    for (auto * pGameObject : mGameObjects)
    {
        delete pGameObject;
    }

    mGameObjects.clear();
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

    for (auto & manager : mManagers)
    {
        manager.second->Update();
    }

    CheckCollision();
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::UpdateGameObjects()
{
    for (int ii = 0; ii < mGameObjects.size();)
    {
        GameObject * pGameObject = mGameObjects[ii];

        if (pGameObject->IsDestroyed())
        {
            pGameObject->CleanUpChildren();
            delete pGameObject;
            mGameObjects.erase(mGameObjects.begin() + ii);
        }
        else
        {
            pGameObject->Update();
            ++ii;
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

    auto * pScoreManager = GetManager<ScoreManager>();
    mpWindow->draw(pScoreManager->GetScoreText());

    auto & spriteLives = pScoreManager->GetSpriteLives();
    for (auto & life : spriteLives)
    {
        mpWindow->draw(life);
    }

    RenderImGui();

    mpWindow->display(); // Renderer is done keep at the end.
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::CheckCollision()
{
    // Retrieve player manager and player object
    auto * pPlayerManager = GetManager<PlayerManager>();
    if (!pPlayerManager || pPlayerManager->GetPlayers().empty())
    {
        return; // No players to check collisions
    }
    GameObject * pPlayer = pPlayerManager->GetPlayers()[0];

    // Iterate through all game objects
    for (int ii = 0; ii < mGameObjects.size(); ++ii)
    {
        GameObject * pObjA = mGameObjects[ii];
        auto pCollA = pObjA->GetComponent<CollisionComponent>().lock();
        if (!pCollA) continue;

        for (int jj = ii + 1; jj < mGameObjects.size(); ++jj)
        {
            GameObject * pObjB = mGameObjects[jj];
            auto pCollB = pObjB->GetComponent<CollisionComponent>().lock();
            if (!pCollB) continue;

            // Ensure one object is the player and teams are different
            if ((pObjA == pPlayer) ^ (pObjB == pPlayer))
            {
                if (pObjA->GetTeam() == pObjB->GetTeam())
                {
                    continue;
                }

                // Check for collision
                if (pCollA->CheckCollision(*pCollB))
                {
                    auto pHealthComp = (pObjA == pPlayer ?
                        pObjA->GetComponent<HealthComponent>().lock() :
                        pObjB->GetComponent<HealthComponent>().lock());

                    if (pHealthComp)
                    {
                        pHealthComp->LooseHealth(100);
                    }
                }
            }
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------

template <typename T>
void GameManager::AddManager()
{
    static_assert(std::is_base_of<BaseManager, T>::value, "T must inherit from BaseManager");
    mManagers[typeid(T)] = new T(this);
}

//------------------------------------------------------------------------------------------------------------------------

template <typename T>
T * GameManager::GetManager()
{
    auto it = mManagers.find(typeid(T));
    if (it != mManagers.end())
    {
        return dynamic_cast<T *>(it->second);
    }
    return nullptr;
}

//------------------------------------------------------------------------------------------------------------------------

GameObject * GameManager::CreateNewGameObject(ETeam team)
{
    GameObject * pGameObj = new GameObject(this, team);
    mGameObjects.push_back(pGameObj);
    return pGameObj;
}

//------------------------------------------------------------------------------------------------------------------------

std::vector<GameObject *> & GameManager::GetGameObjects()
{
    return mGameObjects;
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