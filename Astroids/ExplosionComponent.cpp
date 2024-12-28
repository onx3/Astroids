#include "ExplosionComponent.h"
#include <cassert>
#include <random>
#include "GameObject.h"
#include "SpriteComponent.h"

ExplosionComponent::ExplosionComponent(GameObject * pOwner, const std::string & spriteSheetPath, int frameWidth, int frameHeight, int numFrames, float frameTime)
    : GameComponent(pOwner)
    , mFrameWidth(frameWidth)
    , mFrameHeight(frameHeight)
    , mNumFrames(numFrames)
    , mFrameTime(frameTime)
    , mElapsedTime(0.0f)
    , mCurrentFrame(0)
    , mAnimationFinished(false)
    , mSoundPlayed(false)
{
    if (!mTexture.loadFromFile(spriteSheetPath))
    {
        throw std::runtime_error("Failed to load sprite sheet: " + spriteSheetPath);
    }

    mSprite.setTexture(mTexture);
    mSprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));

    // Sound
    {
        assert(mSoundBuffer.loadFromFile("Audio/explosion.wav"));
        mSound.setBuffer(mSoundBuffer);
        mSound.setVolume(20.f);

        // Randomize pitch between 0.95 and 1.05
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> pitchDist(0.95f, 1.05f);
        float randomPitch = pitchDist(gen);
        mSound.setPitch(randomPitch);
    }
}

//------------------------------------------------------------------------------------------------------------------------

ExplosionComponent::~ExplosionComponent()
{
}

//------------------------------------------------------------------------------------------------------------------------

void ExplosionComponent::Update()
{
    mElapsedTime += mpOwner->GetDeltaTime();

    if (!mSoundPlayed && mCurrentFrame == 0)
    {
        mSound.play();
        mSoundPlayed = true;
    }

    if (mElapsedTime >= mFrameTime && mCurrentFrame < mNumFrames - 1)
    {
        mElapsedTime -= mFrameTime;
        ++mCurrentFrame;

        int columns = mTexture.getSize().x / mFrameWidth;
        int x = (mCurrentFrame % columns) * mFrameWidth;
        int y = (mCurrentFrame / columns) * mFrameHeight;

        mSprite.setTextureRect(sf::IntRect(x, y, mFrameWidth, mFrameHeight));
    }
    else if (mCurrentFrame >= mNumFrames - 1)
    {
        mAnimationFinished = true;
    }
}

//------------------------------------------------------------------------------------------------------------------------

void ExplosionComponent::draw(sf::RenderTarget & target, sf::RenderStates states)
{
    // Draw the explosion sprite
    mSprite.setPosition(mpOwner->GetPosition());
    mSprite.setScale(2.f, 2.f);
    target.draw(mSprite, states);
}

//------------------------------------------------------------------------------------------------------------------------

bool ExplosionComponent::IsAnimationFinished() const
{
    return mAnimationFinished;
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------
