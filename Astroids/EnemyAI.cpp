#include "EnemyAI.h"

EnemyAI::EnemyAI(EEnemy enemyType) : GameObject(), mEnemyType(enemyType)
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
    switch (mEnemyType)
    {
        case EEnemy::Ship:
            SetTexture("Art/enemyShip.png");
            break;
        case EEnemy::Ufo:
            SetTexture("Art/enemyUFO.png");
            break;
    }
}

//------------------------------------------------------------------------------------------------------------------------

void EnemyAI::Update()
{
    // Implement AI-specific behavior here
}
