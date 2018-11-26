#pragma once

#include "Entity.h"
#include "TextureHolder.h"


class Sun // aka root of system
	: public Entity
{
public:
	explicit Sun(TextureHolder& textures);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

	virtual sf::FloatRect getGlobalBounds() const;
	bool collision(sf::Vector2f pos);

private:
	sf::Sprite m_sprite;
	sf::Vector2f m_size;

};