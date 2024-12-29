#pragma once
#include "box2d/box2d.h"
#include "GameObject.h"

class CollisionListener : public b2ContactListener
{
public:
    CollisionListener(GameManager * pGameManager);

    void BeginContact(b2Contact * pContact) override;
    void EndContact(b2Contact * pContact) override;

    void HandleCollision(GameObject * pObjA, GameObject * pObjB);

private:
    GameManager * mpGameManager;
};
