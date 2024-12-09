#include "SpriteComponent.h"
#include "cassert"

SpriteComponent::SpriteComponent(GameObject * pOwner)
    : GameComponent(pOwner)
{
}

//------------------------------------------------------------------------------------------------------------------------

SpriteComponent::~SpriteComponent()
{
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteComponent::SetSprite(const std::string & file)
{
    if (!mTexture.loadFromFile(file))
    {
        assert(false && "Failed to load texture");
    }
    mSprite.setTexture(mTexture);
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

void SpriteComponent::SetRotation(float angle)
{
    mSprite.setRotation(angle);
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

sf::Sprite SpriteComponent::GetSprite()
{
    return mSprite;
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------