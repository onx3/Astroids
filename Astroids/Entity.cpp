#include "Entity.h"

Entity::Entity() : mTexture(), mSprite()
{
}

//------------------------------------------------------------------------------------------------------------------------

sf::Vector2f Entity::GetPosition() const
{
    return mSprite.getPosition();
}

//------------------------------------------------------------------------------------------------------------------------

void Entity::SetPosition(const sf::Vector2f & position)
{
    mSprite.setPosition(position);
}

//------------------------------------------------------------------------------------------------------------------------

void Entity::SetPosition(float x, float y)
{
    mSprite.setPosition(sf::Vector2f(x, y));
}

//------------------------------------------------------------------------------------------------------------------------

float Entity::GetWidth() const
{
    return mSprite.getGlobalBounds().width;
}

//------------------------------------------------------------------------------------------------------------------------

float Entity::GetHeight() const
{
    return mSprite.getGlobalBounds().height;
}

//------------------------------------------------------------------------------------------------------------------------

void Entity::Move(const sf::Vector2f & offset)
{
    mSprite.move(offset);
}

//------------------------------------------------------------------------------------------------------------------------

void Entity::Move(float x, float y)
{
    mSprite.move(sf::Vector2f(x, y));
}

//------------------------------------------------------------------------------------------------------------------------

void Entity::SetRotation(float angle)
{
    mSprite.setRotation(angle);
}

//------------------------------------------------------------------------------------------------------------------------

void Entity::SetOriginToCenter()
{
    mSprite.setOrigin(
        mSprite.getGlobalBounds().width / 2.0f,
        mSprite.getGlobalBounds().height / 2.0f
    );
}

//------------------------------------------------------------------------------------------------------------------------

void Entity::SetTexture(const std::string & file)
{
    if (!mTexture.loadFromFile(file))
    {
        assert(false && "Failed to load texture");
    }
    mSprite.setTexture(mTexture);
}

//------------------------------------------------------------------------------------------------------------------------

void Entity::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

//------------------------------------------------------------------------------------------------------------------------
//EOF
//------------------------------------------------------------------------------------------------------------------------