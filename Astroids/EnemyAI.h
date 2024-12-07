#pragma once
#include "Entity.h"

enum class EEnemy
{
    Ship, 
    Ufo
};

class EnemyAI : public Entity
{
public:
    EnemyAI(EEnemy enemyType);
    ~EnemyAI();

    void Update() override;

private:
    void InitEnemy();
    EEnemy mEnemyType;
};
