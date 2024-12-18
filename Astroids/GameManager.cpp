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
    , mShowImGuiWindow(false)
    , mpRootGameObject(nullptr)
    , mManagers()
    , mCursorTexture()
    , mCursorSprite()
    , mSoundPlayed(false)
{
    mClock.restart();
    InitWindow();

    mpRootGameObject = new GameObject(this, ETeam::Neutral);

    AddManager<PlayerManager>();
    AddManager<EnemyAIManager>();
    AddManager<ScoreManager>();

    // Game Audio
    {
        assert(mSoundBuffer.loadFromFile("Audio/ThroughSpace.ogg"));
        mSound.setBuffer(mSoundBuffer);
        mSound.setVolume(25.f);
        mSound.setLoop(true);
    }
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
    // Game Audio
    if (!mSoundPlayed)
    {
        mSound.play();
        mSoundPlayed = true;
    }

    PollEvents();

    UpdateGameObjects();

    for (auto & manager : mManagers)
    {
        if (manager.second)
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

        CleanUpDestroyedGameObjects(mpRootGameObject);

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

    for (int i = static_cast<int>(children.size()) - 1; i >= 0; --i)
    {
        GameObject * pChild = children[i];

        CleanUpDestroyedGameObjects(pChild);

        if (pChild && pChild->IsDestroyed())
        {
            delete pChild;
            children.erase(children.begin() + i);
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::RenderImGui()
{
#if IMGUI_ENABLED()
    static GameObject * pSelectedGameObject = nullptr;

    ImGui::SFML::Update(*mpWindow, mClock.restart());

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
    {
        mShowImGuiWindow = true;
    }

    if (mShowImGuiWindow && mpRootGameObject)
    {
        ImGui::Begin("Game Objects", &mShowImGuiWindow, ImGuiWindowFlags_NoCollapse);

        ImGui::Columns(2, "GameObjectsColumns", true);

        // LEFT SIDE: GameObject Tree
        ImGui::Text("GameObject Tree");
        ImGui::Separator();

        std::stack<std::pair<GameObject *, int>> stack; // GameObject* + Depth
        stack.push({ mpRootGameObject, 0 });

        while (!stack.empty())
        {
            auto [pGameObject, depth] = stack.top();
            stack.pop();

            if (!pGameObject || pGameObject->IsDestroyed()) continue; // Skip invalid or destroyed objects

            ImGui::Indent(depth * 10.0f);

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

        ImGui::NextColumn();

        // RIGHT SIDE: Components of the Selected GameObject
        ImGui::Text("Components");
        ImGui::Separator();

        if (pSelectedGameObject && !pSelectedGameObject->IsDestroyed())
        {
            ImGui::Text("GameObject %p", pSelectedGameObject);

            for (auto * component : pSelectedGameObject->GetAllComponents())
            {
                std::string componentLabel = "class " + component->GetClassName();
                ImGui::BulletText("%s", componentLabel.c_str());
            }
        }
        else
        {
            ImGui::Text("No GameObject selected or it has been deleted.");
            pSelectedGameObject = nullptr;
        }

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

    // Show mouse cursor if ImGui window is open
    if (!mShowImGuiWindow)
    {
        mpWindow->setMouseCursorVisible(false);
    }

    // Draw background
    mpWindow->draw(mBackgroundSprite);

    // Draw all GameObjects
    if (mpRootGameObject)
    {
        mpWindow->draw(*mpRootGameObject);
    }

    // Draw UI
    {
        auto * pScoreManager = GetManager<ScoreManager>();
        mpWindow->draw(pScoreManager->GetScoreText());

        auto & spriteLives = pScoreManager->GetSpriteLives();
        for (auto & life : spriteLives)
        {
            mpWindow->draw(life);
        }
    }
    
    // Draw Mouse Cursor
    {
        sf::Vector2i mousePosition = sf::Mouse::getPosition(*mpWindow);
        mCursorSprite.setPosition(float(mousePosition.x), float(mousePosition.y));
        mpWindow->draw(mCursorSprite);
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
        return;
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
    if (mManagers.find(typeid(T)) == mManagers.end())
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
    mpWindow = new sf::RenderWindow(sf::VideoMode(1800, 1200), "Game", sf::Style::Default);
    mpWindow->setFramerateLimit(240);

    std::string file = "Art/Background/background2.png";
    assert(mBackgroundTexture.loadFromFile(file));
    mBackgroundSprite.setTexture(mBackgroundTexture);

    // Scale to window size
    mBackgroundSprite.setScale(float(mpWindow->getSize().x) / mBackgroundTexture.getSize().x, float(mpWindow->getSize().y) / mBackgroundTexture.getSize().y);

    assert(mCursorTexture.loadFromFile("Art/Crosshair.png"));
    mCursorSprite.setTexture(mCursorTexture);
    mCursorSprite.setScale(.25f, .25f);

    sf::FloatRect localBounds = mCursorSprite.getLocalBounds();
    mCursorSprite.setOrigin(
        localBounds.width / 2.0f,
        localBounds.height / 2.0f
    );

    ImGui::CreateContext();
    ImGui::SFML::Init(*mpWindow);

    mpWindow->setMouseCursorVisible(false);
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------