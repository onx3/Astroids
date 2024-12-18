#include "GameManager.h"
#include <cassert>
#include <imgui.h>
#include <stack>
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
    , mpRootGameObject(nullptr)
    , mManagers()
{
    mClock.restart();
    InitWindow();

    mpRootGameObject = new GameObject(this, ETeam::Neutral);

    AddManager<PlayerManager>();
    AddManager<EnemyAIManager>();
    AddManager<ScoreManager>();
}

//------------------------------------------------------------------------------------------------------------------------

GameManager::~GameManager()
{
    for (auto & manager : mManagers)
    {
        if (manager.second)
        {
            delete manager.second;
            manager.second = nullptr;
        }
    }

    delete mpWindow;
    ImGui::SFML::Shutdown();
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::EndGame()
{
    if (mpRootGameObject)
    {
        delete mpRootGameObject;
        mpRootGameObject = nullptr;
    }
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
        if (manager.second) // Safety check
        {
            manager.second->Update();
        }
        else
        {
            std::cerr << "Error: Manager " << manager.first.name() << " is nullptr!\n";
        }
    }

    CheckCollision();
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::UpdateGameObjects()
{
    if (mpRootGameObject)
    {
        mpRootGameObject->Update();

        // Clean up destroyed objects
        CleanUpDestroyedGameObjects(mpRootGameObject);

        // If the root itself is destroyed
        if (!mpRootGameObject)
        {
            EndGame();
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::CleanUpDestroyedGameObjects(GameObject * pRoot)
{
    if (!pRoot)
        return;

    auto & children = pRoot->GetChildren();

    // Loop backwards to safely delete children
    for (int i = static_cast<int>(children.size()) - 1; i >= 0; --i)
    {
        GameObject * pChild = children[i];

        // Recursively clean up the child
        CleanUpDestroyedGameObjects(pChild);

        // Check if the child should be destroyed
        if (pChild && pChild->IsDestroyed())
        {
            delete pChild;                         // Delete child
            children.erase(children.begin() + i);  // Remove from vector
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::RenderImGui()
{
#if IMGUI_ENABLED()
    static bool showWindow = false;
    static GameObject * pSelectedGameObject = nullptr;

    ImGui::SFML::Update(*mpWindow, mClock.restart());

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
    {
        showWindow = true;
    }

    if (showWindow && mpRootGameObject)
    {
        ImGui::Begin("Game Objects", &showWindow, ImGuiWindowFlags_NoCollapse);

        // Split the window into two columns
        ImGui::Columns(2, "GameObjectsColumns", true);

        // LEFT SIDE: GameObject Tree
        ImGui::Text("GameObject Tree");
        ImGui::Separator();

        // Use a stack to traverse the GameObject tree iteratively
        std::stack<std::pair<GameObject *, int>> stack; // GameObject* + Depth
        stack.push({ mpRootGameObject, 0 });

        while (!stack.empty())
        {
            auto [pGameObject, depth] = stack.top();
            stack.pop();

            if (!pGameObject || pGameObject->IsDestroyed()) continue; // Skip invalid or destroyed objects

            // Indent GameObjects visually in ImGui based on depth
            ImGui::Indent(depth * 10.0f);

            // Display as a selectable node
            std::string label = "GameObject " + std::to_string(reinterpret_cast<std::uintptr_t>(pGameObject));
            if (ImGui::Selectable(label.c_str(), pSelectedGameObject == pGameObject))
            {
                pSelectedGameObject = pGameObject; // Set this as the selected object
            }

            // Push children to the stack
            for (auto * child : pGameObject->GetChildren())
            {
                stack.push({ child, depth + 1 });
            }

            ImGui::Unindent(depth * 10.0f);
        }

        // Move to the right column
        ImGui::NextColumn();

        // RIGHT SIDE: Components of the Selected GameObject
        ImGui::Text("Components");
        ImGui::Separator();

        // Validate selectedGameObject before accessing it
        if (pSelectedGameObject && !pSelectedGameObject->IsDestroyed())
        {
            ImGui::Text("GameObject %p", pSelectedGameObject);

            // Display components here
            for (auto * component : pSelectedGameObject->GetAllComponents())
            {
                std::string componentLabel = "class " + component->GetClassName(); // Assuming a GetClassName() method
                ImGui::BulletText("%s", componentLabel.c_str());
            }
        }
        else
        {
            ImGui::Text("No GameObject selected or it has been deleted.");
            pSelectedGameObject = nullptr; // Reset the pointer to avoid dangling
        }

        // End the columns
        ImGui::Columns(1);

        ImGui::End();
    }

    ImGui::SFML::Render(*mpWindow);
#endif
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::Render()
{
    mpWindow->clear();

    // Draw background
    mpWindow->draw(mBackgroundSprite);

    // Draw the root GameObject and its children
    if (mpRootGameObject)
    {
        mpWindow->draw(*mpRootGameObject);
    }

    auto * pScoreManager = GetManager<ScoreManager>();
    mpWindow->draw(pScoreManager->GetScoreText());

    auto & spriteLives = pScoreManager->GetSpriteLives();
    for (auto & life : spriteLives)
    {
        mpWindow->draw(life);
    }

    RenderImGui();
    mpWindow->display();
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::CheckCollision()
{
    auto * pPlayerManager = GetManager<PlayerManager>();
    if (!pPlayerManager || pPlayerManager->GetPlayers().empty())
    {
        return; // No players to check collisions
    }
    GameObject * pPlayer = pPlayerManager->GetPlayers()[0];

    CheckCollisionRecursive(mpRootGameObject, pPlayer);
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::CheckCollisionRecursive(GameObject * pRoot, GameObject * pPlayer)
{
    if (!pRoot) return;

    auto pCollA = pRoot->GetComponent<CollisionComponent>().lock();
    if (pCollA && pRoot != pPlayer && pRoot->GetTeam() != pPlayer->GetTeam())
    {
        auto pCollB = pPlayer->GetComponent<CollisionComponent>().lock();
        if (pCollB && pCollA->CheckCollision(*pCollB))
        {
            auto pHealthComp = pPlayer->GetComponent<HealthComponent>().lock();
            if (pHealthComp)
            {
                pHealthComp->LooseHealth(100);
            }
        }
    }

    for (auto * child : pRoot->GetChildren())
    {
        CheckCollisionRecursive(child, pPlayer);
    }
}

//------------------------------------------------------------------------------------------------------------------------

template <typename T>
void GameManager::AddManager()
{
    static_assert(std::is_base_of<BaseManager, T>::value, "T must inherit from BaseManager");
    if (mManagers.find(typeid(T)) == mManagers.end()) // Prevent overwriting existing managers
    {
        mManagers[typeid(T)] = new T(this);
    }
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

GameObject * GameManager::CreateNewGameObject(ETeam team, GameObject * pParent)
{
    GameObject * pGameObj = new GameObject(this, team, pParent);
    return pGameObj;
}

//------------------------------------------------------------------------------------------------------------------------

GameObject * GameManager::GetRootGameObject()
{
    return mpRootGameObject;
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