

#include "Menu.h";



Menu::Menu()
	: m_window(sf::VideoMode(1280, 960), "SFML Application")
	, m_player()
{

}

void Menu::run()
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
			m_player.handlePlayerInput(event.key.code, true);
			break;
		case sf::Event::KeyReleased:
			m_player.handlePlayerInput(event.key.code, false);
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

	if (m_player.isMoving(m_player.UP))
		movement.y -= m_player.getSpeed();

	if (m_player.isMoving(m_player.DOWN))
		movement.y += m_player.getSpeed();

	if (m_player.isMoving(m_player.LEFT))
		movement.x -= m_player.getSpeed();

	if (m_player.isMoving(m_player.RIGHT))
		movement.x += m_player.getSpeed();

	m_player.move(movement * deltaTime.asSeconds());
}

void Game::render()
{

	m_window.clear();
	m_window.draw(m_player.getSprite());
	m_window.display();
}



