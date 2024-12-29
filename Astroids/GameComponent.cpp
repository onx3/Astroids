#include "AstroidsPrivate.h"
#include "GameComponent.h"
#include "imgui.h"
#include "BDConfig.h"

GameComponent::GameComponent(GameObject * pOwner)
    : mpOwner(pOwner)
    , mName("GameComponent")
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

void GameComponent::draw(sf::RenderTarget & target, sf::RenderStates states)
{

}

//------------------------------------------------------------------------------------------------------------------------

void GameComponent::DebugImGuiComponentInfo()
{
#if IMGUI_ENABLED()

#endif
}

//------------------------------------------------------------------------------------------------------------------------

std::string & GameComponent::GetClassName()
{
    return mName;
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------