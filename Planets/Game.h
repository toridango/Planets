#pragma once
#include <SFML/Graphics.hpp>

#include "Player.h"

class Game
{
public:
	Game();
	void run();

private:
	void processEvents();
	void update(sf::Time deltaTime);
	void render();


private:
	sf::RenderWindow m_window;
	Player m_player;

};