#include "Player.h"
#include "cassert"

Player::Player()
	: mTexture()
	, mSprite()
	, mVelocityX(3.f)
	, mVelocityY(3.f)
{
	std::string file = "Art/player.png";
	assert(mTexture.loadFromFile(file), "Can't load file: %s", file.c_str());

	mSprite.setTexture(mTexture);

	SetOriginToCenter();
}

//------------------------------------------------------------------------------------------------------------------------

Player::~Player()
{
}

//------------------------------------------------------------------------------------------------------------------------

sf::Vector2f Player::GetPosition() const
{
	return mSprite.getPosition();
}

//------------------------------------------------------------------------------------------------------------------------

void Player::SetPosition(const sf::Vector2f & pos)
{
	mSprite.setPosition(pos);
}

//------------------------------------------------------------------------------------------------------------------------

float Player::GetWidth()
{
	return mSprite.getGlobalBounds().width;
}

//------------------------------------------------------------------------------------------------------------------------

float Player::GetHeight()
{
	return mSprite.getGlobalBounds().height;
}

//------------------------------------------------------------------------------------------------------------------------

void Player::Move(const sf::Vector2f & offset)
{
	mSprite.move(offset);
}

//------------------------------------------------------------------------------------------------------------------------

void Player::SetRotation(float angle)
{
	mSprite.setRotation(angle);
}

//------------------------------------------------------------------------------------------------------------------------

void Player::SetOriginToCenter()
{
	mSprite.setOrigin(
		mSprite.getGlobalBounds().width / 2.0f,
		mSprite.getGlobalBounds().height / 2.0f);
}

//------------------------------------------------------------------------------------------------------------------------

void Player::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------