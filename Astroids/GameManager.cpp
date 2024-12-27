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

GameManager::GameManager(WindowManager & windowManager)
    : mWindowManager(windowManager)
    , mpWindow(windowManager.GetWindow())
    , mEvent(windowManager.GetEvent())
    , mBackgroundTexture()
    , mBackgroundSprite()
    , mShowImGuiWindow(false)
    , mpRootGameObject(nullptr)
    , mManagers()
    , mCursorTexture()
    , mCursorSprite()
    , mSoundPlayed(false)
    , mIsGameOver(false)
    , mPhysicsWorld(b2Vec2(0.0f, 0.f))
    , mCollisionListener()
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

    // End Game
    {
        assert(mFont.loadFromFile("Art/font.ttf"));

        mGameOverText.setFont(mFont);
        mGameOverText.setString("GAME OVER");
        mGameOverText.setCharacterSize(64);
        mGameOverText.setFillColor(sf::Color::Green);
        mGameOverText.setPosition(700, 400);

        // Setup Score text
        mScoreText.setFont(mFont);
        mScoreText.setCharacterSize(32);
        mScoreText.setFillColor(sf::Color::Green);
        mScoreText.setPosition(700, 500);
    }

    // Box2d
    {
        mPhysicsWorld.SetContactListener(&mCollisionListener);
        mPhysicsWorld.Step(0.f, 0, 0);
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
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::EndGame()
{
    if (mpRootGameObject)
    {
        delete mpRootGameObject;
        mpRootGameObject = nullptr;
    }
    GameOver();
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

    // Physics
    {
        float timeStep = 1.0f / 60.0f; // 60 Hz
        int velocityIterations = 8;
        int positionIterations = 3;
        mPhysicsWorld.Step(timeStep, velocityIterations, positionIterations);
    }

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

            for (auto * pComponent : pSelectedGameObject->GetAllComponents())
            {
                std::string componentLabel = "" + pComponent->GetClassName();

                // Use CollapsingHeader for each component
                if (ImGui::CollapsingHeader(componentLabel.c_str()))
                {
                    pComponent->DebugImGuiComponentInfo();
                }
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
    
    if (mIsGameOver)
    {
        mpWindow->draw(mBackgroundSprite);
        mpWindow->draw(mGameOverText);
        mpWindow->draw(mScoreText);
    }
    else
    {
        // Show mouse cursor if ImGui window is open
        if (!mShowImGuiWindow)
        {
            mpWindow->setMouseCursorVisible(false);
        }
        else
        {
            mpWindow->setMouseCursorVisible(true);
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
    }

    RenderImGui();
    mpWindow->display();
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

bool GameManager::IsGameOver() const
{
    return mIsGameOver;
}

//------------------------------------------------------------------------------------------------------------------------

b2World & GameManager::GetPhysicsWorld()
{
    return mPhysicsWorld;
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::InitWindow()
{
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

    mpWindow->setMouseCursorVisible(false);
}

//------------------------------------------------------------------------------------------------------------------------

void GameManager::GameOver()
{
    mIsGameOver = true;

    auto * pScoreManager = GetManager<ScoreManager>();
    if (pScoreManager)
    {
        mScoreText.setString("Score: " + std::to_string(pScoreManager->GetScore()) + "\n" + "Press ENTER to Play Again!");
    }

    if (mpRootGameObject)
    {
        delete mpRootGameObject;
        mpRootGameObject = nullptr;
    }
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------