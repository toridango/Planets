#pragma once
#include <regex>

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Network.hpp>

#include "Player.h"
#include "Button.h"
#include "Sun.h"
#include "Shot.h"

const unsigned short PORT = 5000;

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
	void addShot(sf::Vector2f iPos, sf::Vector2f dir, bool allied);
	void spawnShot(sf::Vector2i mousePos);
	void incomingShot(float iPosx, float iPosy, float dirx, float diry);
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

	sf::Clock m_clock;
	sf::Font m_font;
	sf::Text m_info;
	std::string m_auxString;
	std::string m_infoHead;
	sf::IpAddress local_ip;
	sf::TcpSocket m_socket;
	sf::Time m_savedTime;
	bool m_connected = false;

};