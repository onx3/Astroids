#include "GameComponent.h"

GameComponent::GameComponent(GameObject * pOwner)
    : mpOwner(pOwner)
{
}

//------------------------------------------------------------------------------------------------------------------------

void GameComponent::SetOwner(GameObject * pOwner)
{
    mpOwner = pOwner;
}

//------------------------------------------------------------------------------------------------------------------------

GameObject & GameComponent::GetGameObject() const
{
    return *mpOwner;
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------