#include "Sun.h"



Sun::Sun(TextureHolder& textures)
	: m_sprite(textures.get(Textures::SUN))
{
	float sizeX = 300.0f;
	float sizeY = 300.0f;
	//m_size = sf::Vector2f(300.0f, 300.0f);
	m_size = sf::Vector2f(0.666 * sizeX, 0.666 * sizeY);
	sf::Texture tex = textures.get(Textures::SUN);
	m_sprite.scale(sf::Vector2f(m_size.x / (float)tex.getSize().x, m_size.y / (float)tex.getSize().y));

	sf::FloatRect bounds = m_sprite.getLocalBounds();
	m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

sf::FloatRect Sun::getGlobalBounds() const
{
	return m_sprite.getGlobalBounds();
}

bool Sun::collision(sf::Vector2f pos)
{
	float distance = sqrt(pow(m_sprite.getPosition().x - pos.x, 2) + pow(m_sprite.getPosition().y - pos.y, 2));

	return distance <= 0.85*(m_sprite.getGlobalBounds().height / 2);
}

void Sun::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Debugging the shape and position of the sprite

	/*sf::FloatRect b = m_sprite.getGlobalBounds();
	sf::ConvexShape convex;
	convex.setPointCount(4);
	convex.setPoint(0, sf::Vector2f(b.left, b.top));
	convex.setPoint(1, sf::Vector2f(b.left + b.width, b.top));
	convex.setPoint(2, sf::Vector2f(b.left + b.width, b.top + b.height));
	convex.setPoint(3, sf::Vector2f(b.left, b.top + b.height));*/

	/*float radius = 0.85*(m_sprite.getGlobalBounds().height) / 2.0;
	sf::CircleShape shape(radius);
	//shape.setOrigin(sf::Vector2f(diameter/2.0, diameter/2.0));
	shape.setPosition(sf::Vector2f(m_sprite.getPosition().x - radius, m_sprite.getPosition().y - radius));
	shape.setFillColor(sf::Color(100, 250, 50));
	target.draw(shape, states);*/

	target.draw(m_sprite, states);
}