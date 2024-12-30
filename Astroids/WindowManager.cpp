#include "AstroidsPrivate.h"
#include "WindowManager.h"
#include <cassert>
#include <imgui-SFML.h>
#include <imgui.h>

WindowManager::WindowManager()
	: mEvent()
{
    mpWindow = new sf::RenderWindow(sf::VideoMode(1800, 1200), "Astroids", sf::Style::Default);
    mpWindow->setFramerateLimit(240);

    mpWindow->setMouseCursorVisible(false);

    ImGui::CreateContext();
    ImGuiIO & io = ImGui::GetIO();
    ImGui::SFML::Init(*mpWindow);
}

//------------------------------------------------------------------------------------------------------------------------

WindowManager::~WindowManager()
{
    ImGui::SFML::Shutdown();
    delete mpWindow;
}

//------------------------------------------------------------------------------------------------------------------------

void WindowManager::PollEvents()
{
    ImGui::SFML::ProcessEvent(mEvent);
    while (mpWindow->pollEvent(mEvent))
    {
        switch (mEvent.type)
        {
            case sf::Event::Closed:
            {
                mpWindow->close();
                break;
            }
            case sf::Event::KeyPressed:
            {
                /*if (mEvent.key.code == sf::Keyboard::Escape)
                {
                    mpWindow->close();
                }*/
                break;
            }
        }
    }
}

//------------------------------------------------------------------------------------------------------------------------

sf::RenderWindow * WindowManager::GetWindow()
{
	return mpWindow;
}

//------------------------------------------------------------------------------------------------------------------------

sf::Event WindowManager::GetEvent() const
{
    return mEvent;
}

//------------------------------------------------------------------------------------------------------------------------
// EOF
//------------------------------------------------------------------------------------------------------------------------