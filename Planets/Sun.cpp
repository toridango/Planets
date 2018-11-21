#include "Sun.h"



Sun::Sun(TextureHolder& textures)
	: m_sprite(textures.get(Textures::SUN))
{
	m_size = sf::Vector2f(300.0, 300.0);
	sf::Texture tex = textures.get(Textures::SUN);
	m_sprite.scale(sf::Vector2f(m_size.x / (float)tex.getSize().x, m_size.y / (float)tex.getSize().y));

	sf::FloatRect bounds = m_sprite.getLocalBounds();
	m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}


void Sun::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}