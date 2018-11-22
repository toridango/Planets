#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Network/IpAddress.hpp>

#include "Player.h"
#include "Button.h"
#include "Sun.h"


class Game
{
public:
	Game();
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
	float insideOrbitRadius;
	float outsideOrbitRadius;
	Player* m_player;
	Planet* m_opponent;
	Planet* m_crossH;
	SceneNode m_sceneGraph;
	BTYPE type;


	sf::Font m_font;
	sf::Text m_info;
	std::string m_inString;
	sf::IpAddress local_ip;

};