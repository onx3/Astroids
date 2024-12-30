#pragma once
#include "AstroidsPrivate.h"
#include "GameComponent.h"

class RandomMovementComponent : public GameComponent
{
public:
    RandomMovementComponent(GameObject * pOwner);
    virtual ~RandomMovementComponent();

    void Update(float deltaTime) override;

    virtual void DebugImGuiComponentInfo() override;

    virtual std::string & GetClassName() override;

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
