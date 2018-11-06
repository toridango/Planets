#pragma once

#include "Game.h"
#include "Button.h"


class Menu
{
public:
	Menu();
	void run();

private:
	void processEvents();
	void update(sf::Time deltaTime);
	void render();


private:
	sf::RenderWindow m_window;

	std::vector<Button*> bArray;
	Button b_host;
	Button b_join;
	Button b_exit;

	sf::Texture m_bg_tex;
	sf::Sprite m_background;

};