#pragma once

class GameObject;

class GameComponent
{
public:
    GameComponent();
    virtual ~GameComponent() = default;

    void SetOwner(GameObject * owner);

    GameObject & GetGameObject() const;

    virtual void Update() = 0;

protected:
    GameObject * mpOwner = nullptr;
};