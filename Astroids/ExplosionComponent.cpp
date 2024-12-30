#include "AstroidsPrivate.h"
#include "ExplosionComponent.h"
#include <cassert>
#include <random>
#include "GameObject.h"
#include "SpriteComponent.h"

ExplosionComponent::ExplosionComponent(GameObject * pOwner, const std::string & spriteSheetPath, int frameWidth, int frameHeight, int numFrames, float frameTime, sf::Vector2f scale, sf::Vector2f pos)
    : GameComponent(pOwner)
    , mFrameWidth(frameWidth)
    , mFrameHeight(frameHeight)
    , mNumFrames(numFrames)
    , mFrameTime(frameTime)
    , mElapsedTime(0.0f)
    , mCurrentFrame(0)
    , mAnimationFinished(false)
    , mScale(scale)
    , mPosition(pos)
    , mSoundPlayed(false)
{
    std::string file = spriteSheetPath;
    ResourceId resourceId(file);

    auto pTexture = mpOwner->GetGameManager().GetManager<ResourceManager>()->GetTexture(resourceId);
    if (pTexture)
    {
        mTexture = *pTexture;
        mSprite.setTexture(mTexture);
        mSprite.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));

        // Want to explosion to be in the center of the positon given
        {
            sf::FloatRect localBounds = mSprite.getLocalBounds();
            mSprite.setOrigin(
                localBounds.width / 2.0f,
                localBounds.height / 2.0f
            );
        }
    }

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

void ExplosionComponent::Update(float deltaTime)
{
    mElapsedTime += deltaTime;

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
    mSprite.setPosition(mPosition);
    mSprite.setScale(mScale);
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
