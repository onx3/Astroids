#include "GameComponent.h"
#include "GameManager.h"
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
    //ImGui::Text("This component doesn't hold any info.");
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