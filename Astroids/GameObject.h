#pragma once
#include "AstroidsPrivate.h"
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>
#include <string>
#include "box2d/box2d.h"

class GameComponent;
class GameManager;

enum class ETeam
{
    Player,
    Friendly,
    Enemy,
    Neutral,
    NukeDrop,
    LifeDrop
};

class GameObject : public sf::Drawable
{
public:
    void Destroy();
    bool IsDestroyed() const;

    void CreatePhysicsBody(b2World * world, const sf::Vector2f & size, bool isDynamic);

    void DestroyPhysicsBody(b2World * world);

    b2Body * GetPhysicsBody() const;

    void NotifyParentOfDeletion();

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

    template <typename T>
    bool HasComponent() const
    {
        return mComponents.find(std::type_index(typeid(T))) != mComponents.end();
    }

    void Update(float deltaTime);

    ETeam GetTeam() const;
    void SetTeam(ETeam team);

    sf::Vector2f GetPosition() const;
    void SetPosition(const sf::Vector2f & position);

    float GetRotation() const;
    void SetRotation(float angle);
    sf::Vector2f GetSize() const;

    GameManager & GetGameManager() const;

    void AddChild(GameObject * pChild);
    void RemoveChild(GameObject * pChild);
    std::vector<GameObject *> & GetChildren();

    GameObject * GetParent() const;
    void SetParent(GameObject * pParent);

    std::vector<GameComponent *> GetAllComponents();

    void SetActiveState(bool active);
    bool IsActive();

    void DebugImGuiInfo();

    const float PIXELS_PER_METER = 100.f;

protected:
    GameObject(GameManager * pGameManager, ETeam team, GameObject * pParent = nullptr);
    ~GameObject();

    void CleanUpChildren();

    virtual void draw(sf::RenderTarget & target, sf::RenderStates states) const override;

    std::unordered_map<std::type_index, std::shared_ptr<GameComponent>> mComponents;

private:
    bool mIsDestroyed; // Used to know when GameManager can Delete
    bool mIsActive; // Used to know when the GameObject is dying
    GameManager * mpGameManager;
    ETeam mTeam;
    std::vector<GameObject *> mChildGameObjects;
    GameObject * mpParent;
    b2Body * mpPhysicsBody;

    friend class GameManager;
};

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------
