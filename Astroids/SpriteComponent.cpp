#include "SpriteComponent.h"
#include "cassert"

SpriteComponent::SpriteComponent(GameObject * pOwner)
    : GameComponent(pOwner)
    , mRotationSpeed(3.f)
    , mCurrentRotation(0.f)
{
}

//------------------------------------------------------------------------------------------------------------------------

SpriteComponent::~SpriteComponent()
{
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteComponent::SetSprite(const std::string & file, const sf::Vector2f & scale)
{
    if (!mTexture.loadFromFile(file))
    {
        assert(false && "Failed to load texture");
    }
    mSprite.setTexture(mTexture);
    mSprite.setScale(scale);
}

//------------------------------------------------------------------------------------------------------------------------

sf::Vector2f SpriteComponent::GetPosition() const
{
    return mSprite.getPosition();
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteComponent::SetPosition(const sf::Vector2f & position)
{
    mSprite.setPosition(position);
}

//------------------------------------------------------------------------------------------------------------------------

float SpriteComponent::GetWidth() const
{
    return mSprite.getGlobalBounds().width;
}

//------------------------------------------------------------------------------------------------------------------------

float SpriteComponent::GetHeight() const
{
    return mSprite.getGlobalBounds().height;
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteComponent::Move(const sf::Vector2f & offset)
{
    mSprite.move(offset);
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteComponent::Move(float x, float y)
{
    mSprite.move(sf::Vector2f(x, y));
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteComponent::RotateClockwise()
{
    mCurrentRotation += mRotationSpeed;
    if (mCurrentRotation >= 360.f)
        mCurrentRotation -= 360.f; // Keep rotation in range
   SetRotation(mCurrentRotation);
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteComponent::RotateCounterClockwise()
{
    mCurrentRotation -= mRotationSpeed;
    if (mCurrentRotation < 0.f)
        mCurrentRotation += 360.f; // Keep rotation in range
    SetRotation(mCurrentRotation);
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteComponent::SetRotation(float angle)
{
    mSprite.setRotation(angle);
}

//------------------------------------------------------------------------------------------------------------------------

float SpriteComponent::GetRotation() const
{
    return mSprite.getRotation();
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteComponent::SetOriginToCenter()
{
    mSprite.setOrigin(
        mSprite.getGlobalBounds().width / 2.0f,
        mSprite.getGlobalBounds().height / 2.0f
    );
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteComponent::Update()
{
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteComponent::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

//------------------------------------------------------------------------------------------------------------------------

sf::Sprite SpriteComponent::GetSprite()
{
    return mSprite;
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------