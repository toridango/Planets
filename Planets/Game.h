#pragma once
#include <regex>
#include <chrono>

#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Network.hpp>

#include "Player.h"
#include "Button.h"
#include "Sun.h"
#include "Shot.h"
#include "Shield.h"

const unsigned short PORT = 53000;

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
	void addShot(sf::Vector2f iPos, sf::Vector2f dir, bool allied, sf::Vector2f realPos);
	void spawnShot(sf::Vector2i mousePos);
	void incomingShot(float iPosx, float iPosy, float dirx, float diry, sf::Int32 ms_remote);
	void updateWorldMap(std::string key, sf::Vector2f);
	void activateShield(bool allied, float angle);
	void checkForOpponent();
	void synchWithHost(long latency, float playerAngPos, float oppoAngPos);
	void handlePacket(sf::Packet packet);
	void sendPendingNotices();
	void setLatencyString(long latency);
	void updateLatencyString(long ms_remote);
	void sendPacket(sf::Packet p);

private:
	sf::RenderWindow m_window;
	TextureHolder m_textures;
	Sun* m_sun;
	float insideOrbitRadius;
	float outsideOrbitRadius;
	float crosshairsRadius;
	float shieldRadius;
	Player* m_player;
	Planet* m_opponent;
	Planet* m_crossH;
	Shield* m_allySh;
	Shield* m_enemySh;
	std::vector<Shot*> m_shots;
	SceneNode m_sceneGraph;
	BTYPE type;
	std::regex ipv4_regex;

	sf::Clock m_clock;
	sf::Font m_font;
	sf::Text m_info;
	sf::Text m_pScore;
	sf::Text m_oScore;
	sf::Text m_latencyInfo;
	std::string m_auxString;
	std::string m_infoHead;
	sf::IpAddress local_ip;
	sf::IpAddress public_ip;
	sf::TcpSocket m_socket;
	sf::TcpListener m_listener;
	sf::Time m_savedTime;
	sf::Socket::Status m_sockStat;

	bool m_connected = false;
	bool m_synched = false;

};