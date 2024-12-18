#pragma once
#include "GameComponent.h"
#include "SFML/Audio.hpp"
#include "string"
#include "GameObject.h"

class ExplosionComponent : public GameComponent
{
public:
	ExplosionComponent(GameObject * owner, const std::string & spriteSheetPath, int frameWidth, int frameHeight, int numFrames, float frameTime);
	~ExplosionComponent();

	virtual void Update() override;

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states) override;

	bool IsAnimationFinished() const;

private:
	sf::Texture mTexture;
	sf::Sprite mSprite;

	int mFrameWidth;
	int mFrameHeight;
	int mNumFrames;
	float mFrameTime;
	float mElapsedTime;
	int mCurrentFrame;
	bool mAnimationFinished;

	// Audio
	sf::SoundBuffer mSoundBuffer;
	sf::Sound mSound;
	bool mSoundPlayed;
};

