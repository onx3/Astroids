#pragma once
#include "GameComponent.h"
#include <SFML/System.hpp>

class RandomMovementComponent : public GameComponent
{
public:
    RandomMovementComponent(GameObject * pOwner);
    virtual ~RandomMovementComponent();

    void Update() override;

private:
    float mVelocity;
    bool mIsEntering;
    bool mSkipBoundaryCheck;
    bool mHasEnteredScreen;
    sf::Vector2f mDirection;
    sf::Vector2f mStartPosition;
    sf::Vector2f mTargetPosition;

    std::string mName;
};

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------
