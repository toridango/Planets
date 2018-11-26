#pragma once
#include <regex>

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Network/IpAddress.hpp>

#include "Player.h"
#include "Button.h"
#include "Sun.h"
#include "Shot.h"


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

	void handleTextEntered(sf::Event e);
	void spawnShot(sf::Vector2i mousePos);
	void updateWorldMap(std::string key, sf::Vector2f);


private:
	sf::RenderWindow m_window;
	TextureHolder m_textures;
	Sun* m_sun;
	float insideOrbitRadius;
	float outsideOrbitRadius;
	Player* m_player;
	Planet* m_opponent;
	Planet* m_crossH;
	std::vector<std::unique_ptr<Shot>> m_shots;
	SceneNode m_sceneGraph;
	BTYPE type;
	std::regex ipv4_regex;

	sf::Font m_font;
	sf::Text m_info;
	std::string m_auxString;
	std::string m_infoHead;
	sf::IpAddress local_ip;

};