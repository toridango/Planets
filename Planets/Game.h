#pragma once
#include <SFML/Graphics.hpp>

#include "Player.h"
#include "Button.h"


class Game
{
public:
	Game();
	Game(BTYPE);
	void run();

private:
	void processEvents();
	void update(sf::Time deltaTime);
	void render();


private:
	sf::RenderWindow m_window;
	Player m_player;
	BTYPE type;

};