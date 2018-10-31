#pragma once

#include "Game.h"

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
	Player m_player;

};