#pragma once
#include <SFML/Graphics.hpp>

#include "TextureHolder.h"

class Player
{
public:
	Player();
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	void move(sf::Vector2f v);

	enum Dir{ UP, DOWN, RIGHT, LEFT };

	bool isMoving(Player::Dir direction);
	float getSpeed();
	sf::Sprite getSprite();


private:
	sf::Texture m_texture;
	sf::Sprite m_sprite;
	sf::Vector2f m_playerSize;
	sf::Vector2u m_texSize;
	bool m_isMovingUp = false;
	bool m_isMovingDown = false;
	bool m_isMovingLeft = false;
	bool m_isMovingRight = false;
	float m_playerSpeed;

};
