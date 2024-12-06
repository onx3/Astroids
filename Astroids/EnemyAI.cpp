#include "EnemyAI.h"
#include "cassert"

//------------------------------------------------------------------------------------------------------------------------

EnemyAI::EnemyAI(const EEnemy & enemyType)
	: mTexture()
	, mSprite()
	, mEnemyType(enemyType)
{
	InitEnemy();
}

//------------------------------------------------------------------------------------------------------------------------

EnemyAI::~EnemyAI()
{

}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAI::InitEnemy()
{
    std::string file;
    switch (mEnemyType)
    {
        case EEnemy::Ship:
            file = "Art/enemyShip.png";
            break;

        case EEnemy::Ufo:
            file = "Art/enemyUFO.png";
            break;
    }

    SetTexture(file);
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAI::SetTexture(const std::string & file)
{
    if (!mTexture.loadFromFile(file))
    {
        //std::cerr << "Error: Could not load texture from " << file << std::endl;
        assert(false);
    }

    mSprite.setTexture(mTexture);
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAI::SetTexture()
{
    mSprite.setTexture(mTexture);
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAI::SetPosition(const sf::Vector2f & vec)
{
	mSprite.setPosition(vec);
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAI::SetPosition(const float x, const float y)
{
	sf::Vector2f vec(x, y);
	SetPosition(vec);
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAI::draw(sf::RenderTarget & target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------