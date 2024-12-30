#include "AstroidsPrivate.h"
#include "SpriteComponent.h"
#include "cassert"
#include "imgui.h"

SpriteComponent::SpriteComponent(GameObject * pOwner)
    : GameComponent(pOwner)
    , mRotationSpeed(3.f)
    , mCurrentRotation(0.f)
    , mFile()
    , mName("SpriteComponent")
{
}

//------------------------------------------------------------------------------------------------------------------------

SpriteComponent::~SpriteComponent()
{
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteComponent::SetSprite(std::shared_ptr<sf::Texture> pTexture, const sf::Vector2f & scale)
{
    
    if (pTexture)
    {
        mSprite.setTexture(*pTexture);
        mSprite.setScale(scale);
    }
    SetOriginToCenter();
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
    sf::FloatRect localBounds = mSprite.getLocalBounds(); // Unscaled, uncropped size of the sprite
    mSprite.setOrigin(
        localBounds.width / 2.0f,
        localBounds.height / 2.0f
    );
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteComponent::Update(float deltaTime)
{
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteComponent::draw(sf::RenderTarget & target, sf::RenderStates states)
{
    if (mpOwner->IsActive())
    {
        target.draw(mSprite, states);
    }
}

//------------------------------------------------------------------------------------------------------------------------

void SpriteComponent::DebugImGuiComponentInfo()
{
    ImGui::Text("File Path: %s", mFile.c_str());
}

//------------------------------------------------------------------------------------------------------------------------

std::string & SpriteComponent::GetClassName()
{
    return mName;
}

//------------------------------------------------------------------------------------------------------------------------

sf::Sprite & SpriteComponent::GetSprite()
{
    return mSprite;
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------