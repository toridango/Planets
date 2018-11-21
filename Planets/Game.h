#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window/Mouse.hpp>

#include "Player.h"
#include "Button.h"
#include "Sun.h"


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

	void loadTextures();
	void buildScene();


private:
	sf::RenderWindow m_window;
	TextureHolder m_textures;
	Sun* m_sun;
	Player* m_player;
	Planet* m_opponent;
	Planet* m_crossH;
	SceneNode m_sceneGraph;
	BTYPE type;


	sf::Font m_font;
	sf::Text m_info;

};