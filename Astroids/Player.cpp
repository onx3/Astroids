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

}

//------------------------------------------------------------------------------------------------------------------------

Player::~Player()
{
}

//------------------------------------------------------------------------------------------------------------------------

void Player::SetPosition(const sf::Vector2f & pos)
{
	mSprite.setPosition(pos);
}

//------------------------------------------------------------------------------------------------------------------------

void Player::Move(const sf::Vector2f & offset)
{
	mSprite.move(offset);
}

//------------------------------------------------------------------------------------------------------------------------

void Player::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------