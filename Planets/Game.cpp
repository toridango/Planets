
#include "Game.h";



Game::Game()
	: m_window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), "SFML Application")
	, m_textures()
	, m_sceneGraph()
	, m_font()
	, m_info()
	, m_pScore()
	, m_oScore()
	, m_latencyInfo()
	, m_clock()
{
	if (!m_font.loadFromFile("../Planets/Media/Fonts/bahnschrift.ttf"))
	{
		throw "Font didn't load!";
	}
	m_info.setFont(m_font);
	m_info.setFillColor(sf::Color(255, 255, 255, 255));
	m_info.setString("");

	m_pScore.setFillColor(sf::Color(255, 255, 255, 255));
	m_pScore.setFont(m_font);
	m_pScore.setPosition(WIN_WIDTH / 15.0f, WIN_HEIGHT / 10.0f);
	m_oScore.setFillColor(sf::Color(255, 255, 255, 255));
	m_oScore.setFont(m_font);
	m_oScore.setPosition(11.0f * WIN_WIDTH / 15.0f, WIN_HEIGHT / 10.0f);

	m_latencyInfo.setFillColor(sf::Color(255, 255, 255, 255));
	m_latencyInfo.setFont(m_font);
	m_latencyInfo.setPosition(WIN_WIDTH / 15.0f, 9.0f * WIN_HEIGHT / 10.0f);

	//m_info.setPosition(650.0, 150.0);
	m_info.setPosition(WIN_WIDTH / 3.0f, WIN_HEIGHT / 10.0f);
	local_ip = sf::IpAddress::getLocalAddress();
	public_ip = sf::IpAddress::getPublicAddress();

	ipv4_regex = std::regex("^((25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)(\.)?){4}$");

	type = Button::getLastButton();


	loadTextures();
	buildScene();
	m_infoHead = "";


	//m_info.setPosition(WIN_WIDTH / 3.0f, WIN_HEIGHT / 10.0f);
	m_auxString = "";
	if (type == BTYPE::BHOST)
	{
		m_infoHead = "Waiting for opponent to join...\nYour Local and Public Ip addresses:\n";
		m_auxString = local_ip.toString() + "\n" + public_ip.toString();
		m_info.setString(m_infoHead + m_auxString);
		render();
		m_listener.setBlocking(false);
		m_listener.listen(PORT);
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
	m_textures.load(Textures::SHIELDPLAYER, "Media/Textures/dirshield_player.png");
	m_textures.load(Textures::SHIELDOPPO, "Media/Textures/dirshield_opponent.png");


}

void Game::buildScene()
{


	//sf::Texture& texture = m_textures.get(Textures::SUN);
	/*insideOrbitRadius = 250.f;
	outsideOrbitRadius = 450.f;*/
	insideOrbitRadius = 0.666 * 250.f;
	outsideOrbitRadius = 0.666 * 450.f;
	crosshairsRadius = 50.0f;
	shieldRadius = 20.0f;
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
	//sf::Vector2f sunPos(790, 500);
	sf::Vector2f sunPos(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f);
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
	m_crossH->setOrbitRadius(crosshairsRadius);
	m_player->attachChild(std::move(crosshairs));

	std::unique_ptr<Shield> allyShield(new Shield(m_textures, true));
	m_allySh = allyShield.get();
	m_allySh->setOrbitRadius(shieldRadius);
	m_player->attachChild(std::move(allyShield));



	std::unique_ptr<Planet> oppo(new Planet(Planets::Lava, m_textures));
	m_opponent = oppo.get();
	// Pos with respect to parent (sun)
	m_opponent->setOrbitRadius(oppoRadius);
	m_sun->attachChild(std::move(oppo));
	SceneNode::worldMap.insert(std::make_pair("opponent", m_opponent->getWorldPosition()));
	SceneNode::worldSizes.insert(std::make_pair("opponent", m_opponent->getGlobalBounds()));

	std::unique_ptr<Shield> enemyShield(new Shield(m_textures, false));
	m_enemySh = enemyShield.get();
	m_enemySh->setOrbitRadius(shieldRadius);
	m_opponent->attachChild(std::move(enemyShield));



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
				if (m_socket.connect(m_auxString, PORT, sf::Time(sf::milliseconds(5000))) == sf::Socket::Done)
				{
					m_infoHead = "Connected!";
					m_connected = true;
					m_socket.setBlocking(false);
				}
				else
				{
					m_infoHead = "Connecion timed out. Check address and try again\n";
				}

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

void Game::addShot(sf::Vector2f iPos, sf::Vector2f dir, bool allied)
{
	std::unique_ptr<Shot> newshot(new Shot(m_textures, iPos, dir, allied));
	//m_shots.push_back(newshot.get()); // TO-Do use this vector of pointers for the collisions // No need, made them work properly
	m_sceneGraph.attachChild(std::move(newshot));
}

void Game::addShot(sf::Vector2f iPos, sf::Vector2f dir, bool allied, sf::Vector2f realPos)
{
	std::unique_ptr<Shot> newshot(new Shot(m_textures, iPos, dir, allied));
	newshot->setRealPos(realPos);
	//m_shots.push_back(newshot.get()); // TO-Do use this vector of pointers for the collisions // No need, made them work properly
	m_sceneGraph.attachChild(std::move(newshot));
}

void Game::spawnShot(sf::Vector2i mousePos)
{
	// Direction of shot
	sf::Vector2f spawnPos = m_crossH->getWorldPosition();
	float y = (float)mousePos.y - m_player->getWorldPosition().y;
	float x = (float)mousePos.x - m_player->getWorldPosition().x;
	float mod = sqrt(x*x + y * y);
	sf::Vector2f dir(x / mod, y / mod);
	sf::Vector2f iPos(spawnPos.x, spawnPos.y);
	sf::Int32 ID = 0;
	//float elapsed = m_clock.getElapsedTime().asSeconds();
	long ms_local = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();


	sf::Packet packetSend;
	packetSend << ID << iPos.x << iPos.y << dir.x << dir.y << ms_local;
	//m_socket.send(packetSend);
	sendPacket(packetSend);

	addShot(iPos, dir, true);
}

void Game::incomingShot(float iPosx, float iPosy, float dirx, float diry, sf::Int32 ms_remote)
{
	// Direction of shot
	sf::Vector2f dir(dirx, diry);
	sf::Vector2f iPos(iPosx, iPosy);
	long ms_local = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();


	// Position prediction with latency into account
	// millisecond difference to second difference
	long latency = ms_local - ms_remote;
	float dt = ((float)(latency)) / 1000;
	sf::Vector2f rPos = iPos + (float)VELMOD * dt * dir;

	setLatencyString(latency);

	if (latency > 120)
	{
		addShot(iPos, dir, false, rPos);
	}
	else
		addShot(rPos, dir, false);
}



void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
		{
			if (m_allySh->getActive())
			{
				float angSpeed = m_allySh->handlePlayerInput(event.key.code, true);
				float angle = m_allySh->getAngle();

				long ms_local = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				sf::Packet packetSend;
				sf::Int32 ID = 4;
				packetSend << ID << angSpeed << angle << ms_local;
				//m_socket.send(packetSend);
				sendPacket(packetSend);
			}
			break;
		}
		case sf::Event::KeyReleased:
		{
			if (m_allySh->getActive())
			{
				float angSpeed = m_allySh->handlePlayerInput(event.key.code, false);
				float angle = m_allySh->getAngle();

				long ms_local = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				sf::Packet packetSend;
				sf::Int32 ID = 4;
				packetSend << ID << angSpeed << angle << ms_local;
				//m_socket.send(packetSend);
				sendPacket(packetSend);
			}
			break;
		}
		case sf::Event::Closed:
			m_window.close();
			break;
		case sf::Event::MouseButtonPressed:
			if (m_connected && m_synched)
			{
				if (event.mouseButton.button == sf::Mouse::Right)
				{
					bool allied = true;
					sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
					float y = (float)mousePos.y - m_player->getWorldPosition().y;
					float x = (float)mousePos.x - m_player->getWorldPosition().x;
					float a = atan2(y, x) * (180 / PI);

					// Mysterious black magicks: reaches this point but doesn't enter the function
					// Problem was compiler optimisations
					activateShield(allied, a);
				}
				else if (event.mouseButton.button == sf::Mouse::Left)
				{
					spawnShot(sf::Mouse::getPosition(m_window));
				}
			}
			break;
		case sf::Event::TextEntered:
			handleTextEntered(event);
			break;
		}
	}
}


void Game::activateShield(bool allied, float angle)
{
	// TO-DO offset time for enemy activation (elapsed time since timestamp)
	// This implies a global clock class is needed
	if (allied)
	{
		float elapsed = m_clock.getElapsedTime().asSeconds();
		long ms_local = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();

		sf::Packet packetSend;
		sf::Int32 ID = 1;
		packetSend << ID << angle << ms_local;
		//m_socket.send(packetSend);
		sendPacket(packetSend);

		m_allySh->setActive(true, angle);
	}
	else if (!allied)
	{
		m_enemySh->setActive(true, angle);
	}
}


void Game::checkForOpponent()
{
	if (m_listener.accept(m_socket) == sf::Socket::Done)
	{
		m_infoHead = "Opponent found:\n";
		m_auxString = m_socket.getRemoteAddress().toString();
		sf::Time savedTime = m_clock.getElapsedTime();
		m_connected = true;
		m_socket.setBlocking(false);
	}
}


// Latency in milliseconds
void Game::synchWithHost(long latency, float playerAngPos, float oppoAngPos)
{
	m_player->synchAngle(playerAngPos, latency);
	m_opponent->synchAngle(oppoAngPos, latency);
}



void Game::updateWorldMap(std::string key, sf::Vector2f value)
{
	std::map<std::string, sf::Vector2f>::iterator it = SceneNode::worldMap.find(key);
	if (it != SceneNode::worldMap.end())
		it->second = value;
}

void Game::sendPacket(sf::Packet p)
{
	sf::Socket::Status s = m_socket.send(p);
	int tries = 1;
	while (s == sf::Socket::Error /*&& tries < 6*/)
	{
		s = m_socket.send(p);
		++tries;
	}
}

void Game::sendPendingNotices()
{
	if (SceneNode::playerHits != 0 || SceneNode::oppoHits != 0)
	{
		sf::Packet packetSend;
		sf::Int32 ID = 3;
		packetSend << ID << SceneNode::playerHits << SceneNode::oppoHits;
		SceneNode::playerHits = 0;
		SceneNode::oppoHits = 0;
		//m_socket.send(packetSend);
		sendPacket(packetSend);
	}
}

void Game::setLatencyString(long latency)
{
	m_latencyInfo.setString(std::to_string(latency) + "ms");
}

void Game::updateLatencyString(long ms_remote)
{
	long ms_local = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	long latency = ms_local - ms_remote;

	setLatencyString(latency);
}

void Game::handlePacket(sf::Packet packet)
{
	sf::Int32 ID, ms_remote;
	float iPosx, iPosy, dirx, diry;
	float angle, angSpeed;
	int theirSelfHits, theirHitsOnMe;
	packet >> ID;
	switch (ID)
	{
	case (0): // Shot
	{
		packet >> iPosx >> iPosy >> dirx >> diry >> ms_remote;
		incomingShot(iPosx, iPosy, dirx, diry, ms_remote);
		break;
	}
	case (1): // Shield
	{
		packet >> angle >> ms_remote;
		bool allied = false;
		activateShield(allied, angle);
		updateLatencyString(ms_remote);
		break;
	}
	case (3): // Hit Notice
	{
		packet >> theirSelfHits >> theirHitsOnMe;
		SceneNode::playerScore += theirSelfHits;
		SceneNode::oppoScore += theirHitsOnMe;
		break;
	}
	case (4): // Shield direction change
	{
		packet >> angSpeed >> angle >> ms_remote;
		bool allied = false;

		long ms_local = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
		long latency = ms_local - ms_remote;

		m_enemySh->synchAngularSpeed(angSpeed, angle, ms_local, ms_remote);
		setLatencyString(latency);
		break;
	}
	case (2): // Initial synch, shouldnt happen here
	default:
		break;
	}
}


void Game::update(sf::Time deltaTime)
{
	m_info.setString(m_infoHead + m_auxString);

	sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
	float y = (float)mousePos.y - m_player->getWorldPosition().y;
	float x = (float)mousePos.x - m_player->getWorldPosition().x;
	float a = atan2(y, x);

	m_sceneGraph.update(deltaTime);
	updateWorldMap("player", m_player->getWorldPosition());
	updateWorldMap("opponent", m_opponent->getWorldPosition());
	m_sceneGraph.removeWrecks();
	m_crossH->setAngle(a*PI);

	if (m_connected && m_synched)
	{

		m_pScore.setString("Player Score:\n" + std::to_string(SceneNode::playerScore));
		m_oScore.setString("Opponent Score:\n" + std::to_string(SceneNode::oppoScore));

		sf::Packet packetReceive;
		if (m_socket.receive(packetReceive) == sf::Socket::Status::Done)
		{
			handlePacket(packetReceive);
		}

		sendPendingNotices();

		sf::Vector2f movement(0.0f, 0.0f);
		/*if (m_player->isMoving(m_player->UP))
			movement.y -= m_player->getSpeed();

		if (m_player->isMoving(m_player->DOWN))
			movement.y += m_player->getSpeed();

		if (m_player->isMoving(m_player->LEFT))
			movement.x -= m_player->getSpeed();

		if (m_player->isMoving(m_player->RIGHT))
			movement.x += m_player->getSpeed();*/


		//m_player->move(deltaTime.asSeconds());
		//m_opponent->move(deltaTime.asSeconds());

		// Warning: This will delete the strings every update if they're ever used for something else
		if ((m_clock.getElapsedTime() - m_savedTime).asSeconds() > 2.0)
		{
			m_infoHead = m_auxString = "";
		}

	}
	else
	{
		if (!m_connected)
		{
			if (type == BTYPE::BHOST) checkForOpponent();
		}
		if (m_connected && !m_synched)
		{
			if (type == BTYPE::BHOST)
			{
				long ms_local = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
				sf::Packet packetSend;
				sf::Int32 ID = 2;
				packetSend << ID << m_player->getAngle() << m_opponent->getAngle() << ms_local;
				//m_socket.send(packetSend);
				sendPacket(packetSend);
				m_synched = true;
			}
			else if (type == BTYPE::BJOIN)
			{

				sf::Int32 ID;
				int ms_remote, ms_local;
				sf::Packet packetSynch;
				float playerAngPos, oppoAngPos;
				if (m_socket.receive(packetSynch) == sf::Socket::Status::Done)
				{
					packetSynch >> ID >> oppoAngPos >> playerAngPos >> ms_remote;
					ms_local = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
					long latency = ms_local - ms_remote;
					synchWithHost(latency, playerAngPos, oppoAngPos);
					m_synched = true;
				}

			}
		}

		//m_auxString = std::to_string(m_sceneGraph.getChildrenCount());
	}
}
void Game::render()
{

	m_window.clear();
	//m_window.draw(m_player.getSprite());
	m_window.draw(m_sceneGraph);
	m_window.draw(m_info);
	m_window.draw(m_pScore);
	m_window.draw(m_oScore);
	m_window.draw(m_latencyInfo);
	m_window.display();
}



