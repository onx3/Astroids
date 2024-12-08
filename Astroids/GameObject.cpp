#include "GameObject.h"
#include "cassert"

GameObject::GameObject() : mTexture(), mSprite()
{
}

//------------------------------------------------------------------------------------------------------------------------

sf::Vector2f GameObject::GetPosition() const
{
    return mSprite.getPosition();
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::SetPosition(const sf::Vector2f & position)
{
    mSprite.setPosition(position);
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::SetPosition(float x, float y)
{
    mSprite.setPosition(sf::Vector2f(x, y));
}

//------------------------------------------------------------------------------------------------------------------------

float GameObject::GetWidth() const
{
    return mSprite.getGlobalBounds().width;
}

//------------------------------------------------------------------------------------------------------------------------

float GameObject::GetHeight() const
{
    return mSprite.getGlobalBounds().height;
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::Move(const sf::Vector2f & offset)
{
    mSprite.move(offset);
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::Move(float x, float y)
{
    mSprite.move(sf::Vector2f(x, y));
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::SetRotation(float angle)
{
    mSprite.setRotation(angle);
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::SetOriginToCenter()
{
    mSprite.setOrigin(
        mSprite.getGlobalBounds().width / 2.0f,
        mSprite.getGlobalBounds().height / 2.0f
    );
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::AddComponent(std::shared_ptr<GameComponent> component)
{

}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::SetTexture(const std::string & file)
{
    if (!mTexture.loadFromFile(file))
    {
        assert(false && "Failed to load texture");
    }
    mSprite.setTexture(mTexture);
}

//------------------------------------------------------------------------------------------------------------------------

void GameObject::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
    target.draw(mSprite, states);
}

//------------------------------------------------------------------------------------------------------------------------
//EOF
//------------------------------------------------------------------------------------------------------------------------