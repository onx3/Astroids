#pragma once
#include "GameComponent.h"

class DropMovementComponent : public GameComponent
{
public:
	DropMovementComponent(GameObject * pGameOwner);

	void Update(float deltaTime);
	void DebugImGuiComponentInfo() override;
	std::string & GetClassName() override;

private:
	sf::Vector2f mDirection;
	float mVelocity; 
	sf::Vector2f mStartPosition;
	std::string mName;
};

