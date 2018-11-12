#pragma once

#include "Entity.h"


class Planet
	: public Entity
{
public:
	explicit Planet();
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Sprite m_sprite;

};