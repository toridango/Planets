#pragma once
#include <SFML/Graphics.hpp>

#include "Planet.h"
#include "TextureHolder.h"

class Player
	: public Planet
{
public:
	explicit Player(Planets::Type type, TextureHolder& textures);
	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	//void move(sf::Vector2f v);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	
	enum Dir{ UP, DOWN, RIGHT, LEFT };

	bool isMoving(Player::Dir direction);
	float getSpeed();
	sf::Sprite getSprite();



private:
	sf::Vector2f m_playerSize;
	sf::Vector2u m_texSize;
	//float m_playerSpeed;
	bool m_isMovingUp = false;
	bool m_isMovingDown = false;
	bool m_isMovingLeft = false;
	bool m_isMovingRight = false;


};
