
#include "Player.h";
//#include <iostream>



Player::Player()
	: m_sprite()
{

	TextureHolder textures;
	textures.load(Textures::PLANETTERRAN, "Media/Textures/PlanetTerran1.png");

	// HARDCODED STUFF -----------------------------

	m_playerSpeed = 100.0f;
	m_playerSize = sf::Vector2f(50.0, 50.0);

	// ---------------------------------------------

	m_texture = textures.get(Textures::PLANETTERRAN);
	m_sprite.setTexture(m_texture);
	m_texSize = m_texture.getSize();

	//std::cout << texSize.x << " " << texSize.y;
	m_sprite.scale(sf::Vector2f(m_playerSize.x / (float)m_texSize.x, m_playerSize.y / (float)m_texSize.y));
	m_sprite.setPosition(100.0f, 100.0f);
}


void Player::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::W)
		m_isMovingUp = isPressed;
	else if (key == sf::Keyboard::S)
		m_isMovingDown = isPressed;
	else if (key == sf::Keyboard::A)
		m_isMovingLeft = isPressed;
	else if (key == sf::Keyboard::D)
		m_isMovingRight = isPressed;
}

void Player::move(sf::Vector2f v)
{
	m_sprite.move(v);
}

float Player::getSpeed()
{
	return m_playerSpeed;
}

sf::Sprite Player::getSprite()
{
	return m_sprite;
}

bool Player::isMoving(Player::Dir direction)
{
	bool isIt = false;
	switch (direction)
	{
		case UP:
			isIt = m_isMovingUp;
			break;
		case DOWN:
			isIt = m_isMovingDown;
			break;
		case LEFT:
			isIt = m_isMovingLeft;
			break;
		case RIGHT:
			isIt = m_isMovingRight;
			break;
		default:
			break;
	}

	return isIt;
}