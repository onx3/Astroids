#pragma once

#include "Entity.h"
#include <SFML/Window/Keyboard.hpp>
#include <vector>
#include <memory>
#include "GameComponent.h"

class Player : public Entity
{
public:
    Player();
    ~Player();

    void Update() override;

private:
    float mVelocityX;
    float mVelocityY;
    std::vector<std::shared_ptr<GameComponent>> mComponents;
};