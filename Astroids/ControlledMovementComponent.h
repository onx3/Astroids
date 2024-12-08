#pragma once

#include "GameComponent.h"

class ControlledMovementComponent : public GameComponent
{
public:
	ControlledMovementComponent();
	ControlledMovementComponent(float veloX, float veloY);

	~ControlledMovementComponent();

	void Update() override;

	void SetVelocityX(float velo);
	void SetVelocityY(float velo);
private:
	float mVelocityX;
	float mVelocityY;
};

