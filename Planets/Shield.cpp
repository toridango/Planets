#include "Shield.h"




Shield::Shield(TextureHolder& textures, bool allied)
	: m_sprite(textures.get(allied ? Textures::SHIELDPLAYER : Textures::SHIELDOPPO))
{
	float sizeX = 60.0f;
	float sizeY = 60.0f;
	//m_size = sf::Vector2f(60.0, 60.0);
	m_size = sf::Vector2f(0.666 * sizeX, 0.666 * sizeY);
	sf::Texture tex = textures.get(allied ? Textures::SHIELDPLAYER : Textures::SHIELDOPPO);
	m_sprite.scale(sf::Vector2f(m_size.x / (float)tex.getSize().x, m_size.y / (float)tex.getSize().y));

	sf::FloatRect bounds = m_sprite.getLocalBounds();
	m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	m_active = false;
	m_state = OFF;
}


void Shield::setActive(bool active)
{
	if (active && m_state == OFF)
	{
		m_clock.restart();
		m_state = ON;
		m_active = active;
	}
	else if (!active)
		m_active = active; 
}
bool Shield::getActive() const { return m_active; }


void Shield::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_active)
		target.draw(m_sprite, states);
}



void Shield::updateCurrent(sf::Time dt)
{

	sf::Vector2f v = getParentPosition() - getWorldPosition(); // Is this how to fix the other collisions?
	setPosition(v.x, v.y);

	switch (m_state)
	{
	case ON:
		if (m_clock.getElapsedTime().asSeconds() > DURATION)
		{
			m_active = false;
			m_state = CD;
		}
		break;
	case CD:
		if (m_clock.getElapsedTime().asSeconds() > (DURATION + COOLDOWN))
		{
			m_state = OFF;
		}
	case OFF:
	default:
		break;
	}

}

