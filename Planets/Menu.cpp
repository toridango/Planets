#include "Menu.h";



Menu::Menu()
	: m_window(sf::VideoMode(1280, 960), "SFML Application")
	, b_host(BHOST), b_join(BJOIN), b_exit(EXIT)
	, m_background()
{
	if (!m_bg_tex.loadFromFile("../Planets/Media/eclipse.jpg"))
	{
		throw "Image didn't load!";
	}

	m_background.setTexture(m_bg_tex);
	//m_background.setPosition(1280.0 / 2.0 - float(m_bg_tex.getSize().x) / 2.0, 960.0 / 2.0 - float(m_bg_tex.getSize().y) / 2.0);
	m_background.setPosition(-100, -100);

	b_host.setText("Host game");
	//b_host.setSize(sf::Vector2f(200.0f, 20.0f));
	b_host.setPosition(sf::Vector2f(50.0f, 350.0f));
	bArray.push_back(&b_host);

	b_join.setText("Join game");
	b_join.setPosition(sf::Vector2f(50.0f, 500.0f));
	bArray.push_back(&b_join);

	b_exit.setText("Exit");
	b_exit.setPosition(sf::Vector2f(50.0f, 650.0f));
	bArray.push_back(&b_exit);
}

void Menu::run()
{
	bool clicking = false;
	BTYPE exit = NONE;
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
	while (m_window.isOpen())
	{
		exit = NONE;
		processEvents();
		timeSinceLastUpdate += clock.restart();

		for (Button* b : bArray)
		{
			if (b->isMouseOverlap(sf::Mouse::getPosition(m_window)))
			{
				b->highlight(true);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					if (!clicking)
					{
						clicking = true;
						exit = b->getBType();
						b->setLastButton();
						clicking = false;
					}
				}
			}
			else
				b->highlight(false);
		}

		if(exit == EXIT)
		{
			m_window.close();
		}
		else if (exit == BHOST || exit == BJOIN)
		{
			//m_window.close();
			Game game;
			game.run();
		}

		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents();
			update(timePerFrame);
		}
		render();
	}
}


void Menu::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			m_window.close();
			break;
		}
	}
}

void Menu::update(sf::Time deltaTime)
{

}

void Menu::render()
{

	m_window.clear();
	m_window.draw(m_background);
	for (Button* b : bArray)
	{
		m_window.draw(b->getSprite());
		m_window.draw(b->getText());
	}
	m_window.display();
}



