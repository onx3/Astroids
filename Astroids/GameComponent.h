#pragma once

class GameComponent
{
public:
    virtual ~GameComponent() = default;

    virtual void Update() = 0;
};