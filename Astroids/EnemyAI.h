#pragma once
#include "GameObject.h"

enum class EEnemy
{
    Ship, 
    Ufo
};

class EnemyAI : public GameObject
{
public:
    EnemyAI(EEnemy enemyType);
    ~EnemyAI();

    void Update() override;

private:
    void InitEnemy();
    EEnemy mEnemyType;
};
