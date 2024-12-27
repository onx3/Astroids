#pragma once

#include "SFML/Graphics.hpp"

class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	void PollEvents();

	sf::RenderWindow * GetWindow();
	sf::Event GetEvent() const;

private:
	sf::RenderWindow * mpWindow;
	sf::Event mEvent;
};