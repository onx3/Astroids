#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>
#include <string>

class GameComponent;
class GameManager;

enum class ETeam
{
    Friendly,
    Enemy,
    Neutral
};

class GameObject : public sf::Drawable
{
public:
    void Destroy();
    bool IsDestroyed() const;

    // Add a single component of type T
    template <typename T>
    void AddComponent(std::shared_ptr<T> component)
    {
        static_assert(std::is_base_of<GameComponent, T>::value, "T must derive from GameComponent");
        mComponents[typeid(T)] = component; // Store shared ownership
    }

    // Get a single component of type T as a weak_ptr
    template <typename T>
    std::weak_ptr<T> GetComponent() const
    {
        auto it = mComponents.find(typeid(T));
        if (it != mComponents.end())
        {
            return std::weak_ptr<T>(std::static_pointer_cast<T>(it->second));
        }
        return std::weak_ptr<T>(); // Return an empty weak_ptr if the component doesn't exist
    }

    void Update();

    float GetDeltaTime() const;

    ETeam GetTeam() const;
    void SetTeam(ETeam team);

    sf::Vector2f GetPosition() const;
    void SetPosition(const sf::Vector2f & position);

    float GetRotation() const;
    sf::Vector2f GetSize() const;

    GameManager & GetGameManager() const;

    void AddChild(GameObject * pChild);
    std::vector<GameObject *> & GetChildren();

    std::vector<GameComponent *> GetAllComponents();

    void DebugImGuiInfo();

protected:
    GameObject(GameManager * pGameManager, ETeam team, GameObject * pParent = nullptr);
    ~GameObject();

    void CleanUpChildren();

    virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

    std::unordered_map<std::type_index, std::shared_ptr<GameComponent>> mComponents;

private:
    sf::Clock mClock;
    float mDeltaTime;
    bool mIsDestroyed;
    GameManager * mpGameManager;
    ETeam mTeam;
    std::vector<GameObject *> mChildGameObjects;

    friend class GameManager;
};

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------
