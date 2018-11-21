
#include "Game.h";



Game::Game()
	: m_window(sf::VideoMode(1580, 1000), "SFML Application")
	, m_textures()
	, m_sceneGraph()
	, m_font()
	, m_info()
{
	if (!m_font.loadFromFile("Media/Fonts/bahnschrift.ttf"))
	{
		throw "Font didn't load!";
	}
	// TODO: Info text not showing?
	m_info.setFont(m_font);
	m_info.setFillColor(sf::Color(0, 0, 0, 255));
	m_info.setString("");
	m_info.setPosition(100.0, 500.0);

	loadTextures();
	buildScene();
}

Game::Game(BTYPE gt)
	: Game()
{
	type = gt;
}

void Game::loadTextures()
{
	m_info.setString("Loading textures...");

	m_textures.load(Textures::SUN, "Media/Textures/sun.png");
	m_textures.load(Textures::PLANETTERRAN, "Media/Textures/PlanetTerran1.png");
	m_textures.load(Textures::PLANETCITY, "Media/Textures/PlanetCity1.png");
	m_textures.load(Textures::PLANETICE, "Media/Textures/PlanetIce1.png");
	m_textures.load(Textures::PLANETLAVA, "Media/Textures/PlanetLava1.png");
	m_textures.load(Textures::CROSSHAIRS, "Media/Textures/crosshairs.png");

	m_info.setString("");
}

void Game::buildScene()
{

	sf::Texture& texture = m_textures.get(Textures::SUN);

	std::unique_ptr<Sun> sun(new Sun(m_textures));
	m_sun = sun.get();
	// Pos with respect to parent (root)
	m_sun->setPosition(790, 500);
	m_sceneGraph.attachChild(std::move(sun));

	std::unique_ptr<Player> terra(new Player(Planets::Terran, m_textures));
	m_player = terra.get();
	// Pos with respect to parent (sun)
	m_player->setRadius(250.0); // (150.0, 150.0);
	m_sun->attachChild(std::move(terra));
	
	std::unique_ptr<Planet> crosshairs(new Planet(Planets::Crosshairs, m_textures));
	m_crossH = crosshairs.get();
	// Pos with respect to parent (player)
	m_crossH->setRadius(50.0);
	m_player->attachChild(std::move(crosshairs));

	std::unique_ptr<Planet> oppo(new Planet(Planets::Ice, m_textures));
	m_opponent = oppo.get();
	// Pos with respect to parent (sun)
	m_opponent->setRadius(450.0);
	m_sun->attachChild(std::move(oppo));

	
	/* Master TODO:
	*	- Add lasers
	*		AND/OR
	*	- Set up networking
	*
	*	Others:
	*	- info text
	*	- asteroids
	*/


}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
	while (m_window.isOpen())
	{
		processEvents();
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents();
			update(timePerFrame);
		}
		render();
	}
}


void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			m_player->handlePlayerInput(event.key.code, true);
			break;
		case sf::Event::KeyReleased:
			m_player->handlePlayerInput(event.key.code, false);
			break;
		case sf::Event::Closed:
			m_window.close();
			break;
		}
	}
}

void Game::update(sf::Time deltaTime)
{
	sf::Vector2f movement(0.0f, 0.0f);
	sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);

	/*if (m_player->isMoving(m_player->UP))
		movement.y -= m_player->getSpeed();

	if (m_player->isMoving(m_player->DOWN))
		movement.y += m_player->getSpeed();

	if (m_player->isMoving(m_player->LEFT))
		movement.x -= m_player->getSpeed();

	if (m_player->isMoving(m_player->RIGHT))
		movement.x += m_player->getSpeed();*/

	float y = (float) mousePos.y - m_player->getWorldPosition().y;
	float x = (float) mousePos.x - m_player->getWorldPosition().x;

	float a = atan2(y, x);

	m_player->move(deltaTime.asSeconds());
	m_opponent->move(deltaTime.asSeconds());
	m_crossH->setAngle(a*PI);
}

void Game::render()
{

	m_window.clear();
	//m_window.draw(m_player.getSprite());
	m_window.draw(m_sceneGraph);
	m_window.display();
}



