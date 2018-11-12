#pragma once

#include "Entity.h"


class Sun
	: public Entity
{
public:
	explicit Sun();
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Sprite m_sprite;

};