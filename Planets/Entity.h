#pragma once

#include <SFML/Graphics.hpp>

#include "SceneNode.h"
#include "TextureHolder.h"

class Entity
	: public SceneNode
{
public:
	Entity();

	void setVelocity(sf::Vector2f v);
	void setVelocity(float vx, float vy);

	sf::Vector2f getVelocity() const;

private:
	sf::Vector2f m_velocity;
};