#pragma once

#include "AstroidsPrivate.h"

class GameObject;
class GameManager;

class GameComponent
{
public:
    explicit GameComponent(GameObject * pOwner);
    virtual ~GameComponent() = default;

    void SetOwner(GameObject * pOwner);

    GameObject & GetGameObject() const;

    virtual void Update(float deltaTime) = 0;

    virtual void draw(sf::RenderTarget & target, sf::RenderStates states);

    virtual void DebugImGuiComponentInfo();

    virtual std::string & GetClassName();
protected:
    GameObject * mpOwner = nullptr;
private:
    std::string mName;
};

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------