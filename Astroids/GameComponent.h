#pragma once

class GameObject;

class GameComponent
{
public:
    explicit GameComponent(GameObject * pOwner);
    virtual ~GameComponent() = default;

    void SetOwner(GameObject * owner);

    GameObject & GetGameObject() const;

    virtual void Update() = 0;

protected:
    GameObject * mpOwner = nullptr;
};