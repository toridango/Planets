
#include "Game.h";



Game::Game()
	: m_window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "SFML Application")
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
	m_info.setFillColor(sf::Color(255, 255, 255, 255));
	m_info.setString("");
	m_info.setPosition(650.0, 150.0);
	local_ip = sf::IpAddress::getLocalAddress();

	ipv4_regex = std::regex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\.)?){4}$");

	type = Button::getLastButton();

	loadTextures();
	buildScene();
	m_infoHead = "";


	m_info.setPosition(550.0, 150.0);
	m_auxString = "";
	if (type == BTYPE::BHOST)
	{
		m_infoHead = "Waiting for opponent to join...\nYour Ip address: ";
		m_auxString = local_ip.toString();
		m_info.setString(m_infoHead + m_auxString);
	}
	else
	{
		m_infoHead = "Enter host IP address\n";
		m_info.setString(m_infoHead);
	}
}


void Game::loadTextures()
{
	m_info.setString("Loading textures...");
	m_window.draw(m_info);
	m_window.display();

	m_textures.load(Textures::SUN, "Media/Textures/sun.png");
	m_textures.load(Textures::PLANETTERRAN, "Media/Textures/PlanetTerran1.png");
	m_textures.load(Textures::PLANETCITY, "Media/Textures/PlanetCity1.png");
	m_textures.load(Textures::PLANETICE, "Media/Textures/PlanetIce1.png");
	m_textures.load(Textures::PLANETLAVA, "Media/Textures/PlanetLava1.png");
	m_textures.load(Textures::CROSSHAIRS, "Media/Textures/crosshairs.png");
	m_textures.load(Textures::LASERPLAYER, "Media/Textures/beam_player.png");
	m_textures.load(Textures::LASEROPPO, "Media/Textures/beam_opponent.png");


}

void Game::buildScene()
{


	sf::Texture& texture = m_textures.get(Textures::SUN);
	insideOrbitRadius = 250.f;
	outsideOrbitRadius = 450.f;
	float playerRadius;
	float oppoRadius;

	if (type == BTYPE::BHOST)
	{
		playerRadius = insideOrbitRadius;
		oppoRadius = outsideOrbitRadius;
	}
	else
	{
		oppoRadius = insideOrbitRadius;
		playerRadius = outsideOrbitRadius;
	}

	std::unique_ptr<Sun> sun(new Sun(m_textures));
	m_sun = sun.get();
	// Pos with respect to parent (root)
	sf::Vector2f sunPos(790, 500);
	m_sun->setPosition(sunPos.x, sunPos.y);
	m_sceneGraph.attachChild(std::move(sun));
	SceneNode::worldMap.insert(std::make_pair("sun", sunPos));
	SceneNode::worldSizes.insert(std::make_pair("sun", m_sun->getGlobalBounds()));
	//SceneNode::worldSizes.insert(std::make_pair("sun", m_sun->getWorldTransform().transformRect(m_sun->getGlobalBounds())));


	std::unique_ptr<Player> terra(new Player(Planets::Terran, m_textures));
	m_player = terra.get();
	// Pos with respect to parent (sun)
	m_player->setOrbitRadius(playerRadius); // (150.0, 150.0);
	m_sun->attachChild(std::move(terra));
	SceneNode::worldMap.insert(std::make_pair("player", m_player->getWorldPosition()));
	SceneNode::worldSizes.insert(std::make_pair("player", m_player->getGlobalBounds()));

	std::unique_ptr<Planet> crosshairs(new Planet(Planets::Crosshairs, m_textures));
	m_crossH = crosshairs.get();
	// Pos with respect to parent (player)
	m_crossH->setOrbitRadius(50.0);
	m_player->attachChild(std::move(crosshairs));

	std::unique_ptr<Planet> oppo(new Planet(Planets::Lava, m_textures));
	m_opponent = oppo.get();
	// Pos with respect to parent (sun)
	m_opponent->setOrbitRadius(oppoRadius);
	m_sun->attachChild(std::move(oppo));
	SceneNode::worldMap.insert(std::make_pair("opponent", m_opponent->getWorldPosition()));
	SceneNode::worldSizes.insert(std::make_pair("opponent", m_opponent->getGlobalBounds()));


	/* Master TODO:
	*	- Set up networking
	*
	*	Others:
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

void Game::handleTextEntered(sf::Event e)
{
	if (type == BJOIN)
	{
		//if (event.text.unicode == sf::Keyboard::BackSpace && m_inString.size() != 0)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::BackSpace) && m_auxString.size() != 0)
		{
			m_auxString.pop_back();
		}
		else if (((e.text.unicode < 58 && e.text.unicode > 47) || e.text.unicode == 46) && m_auxString.size() + 1 <= 15)
		{
			m_auxString.push_back((char)e.text.unicode);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Enter))
		{
			bool pass = std::regex_match(m_auxString, ipv4_regex);
			if (pass)
			{
				m_infoHead = "";
				m_auxString = "";
				pass = false;
			}
			else
			{
				m_infoHead = "Wrong IPv4 format. Enter again\n";
				m_auxString = "";
			}
		}
	}
}


void Game::spawnShot(sf::Vector2i mousePos)
{
	// Direction of shot
	sf::Vector2f spawnPos = m_crossH->getWorldPosition();
	float y = (float)mousePos.y - m_player->getWorldPosition().y;
	float x = (float)mousePos.x - m_player->getWorldPosition().x;
	float mod = sqrt(x*x + y*y);
	sf::Vector2f dir(x / mod, y / mod);

	sf::Vector2f iPos(spawnPos.x, spawnPos.y);

	std::unique_ptr<Shot> newshot(new Shot(m_textures, iPos, dir, true));
	m_sceneGraph.attachChild(std::move(newshot));
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
		case sf::Event::MouseButtonPressed:
			spawnShot(sf::Mouse::getPosition(m_window));
			break;
		case sf::Event::TextEntered:
			handleTextEntered(event);
			break;
		}
	}
}

void Game::updateWorldMap(std::string key, sf::Vector2f value)
{
	std::map<std::string, sf::Vector2f>::iterator it = SceneNode::worldMap.find(key);
	if (it != SceneNode::worldMap.end())
		it->second = value;
}

void Game::update(sf::Time deltaTime)
{

	m_info.setString(m_infoHead + m_auxString);



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

	float y = (float)mousePos.y - m_player->getWorldPosition().y;
	float x = (float)mousePos.x - m_player->getWorldPosition().x;

	float a = atan2(y, x);


	//m_player->move(deltaTime.asSeconds());
	//m_opponent->move(deltaTime.asSeconds());
	m_crossH->setAngle(a*PI);
	m_sceneGraph.update(deltaTime);
	updateWorldMap("player", m_player->getWorldPosition());
	updateWorldMap("opponent", m_opponent->getWorldPosition());

	m_sceneGraph.removeWrecks();
	//m_auxString = std::to_string(m_sceneGraph.getChildrenCount());
}

void Game::render()
{

	m_window.clear();
	//m_window.draw(m_player.getSprite());
	m_window.draw(m_sceneGraph);
	m_window.draw(m_info);
	m_window.display();
}



