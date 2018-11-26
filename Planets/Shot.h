#pragma once

#include "Entity.h"

constexpr auto VELMOD = 200.0;

class Shot 
	: public Entity
{
public:
	explicit Shot(TextureHolder& textures, sf::Vector2f origin, sf::Vector2f velocity, bool allied);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void updateCurrent(sf::Time dt) override;
	bool getAllied();

private:
	bool sunCollision();
	bool oppoCollision();

private:
	sf::Sprite m_sprite;
	sf::Vector2f m_size;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	bool m_allied;

};