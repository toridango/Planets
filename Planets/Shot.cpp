

#include "Shot.h"



Shot::Shot(TextureHolder& textures, sf::Vector2f origin, sf::Vector2f velocity, bool allied)
	: m_sprite(textures.get(allied ? Textures::LASERPLAYER : Textures::LASEROPPO))
{
	m_position = origin;
	m_velocity = velocity;
	m_allied = allied;

	float sizeX = 10.0f;
	float sizeY = 30.0f;

	sf::Texture tex = textures.get(allied ? Textures::LASERPLAYER : Textures::LASEROPPO);

	//m_size = sf::Vector2f(10.0, 30.0);
	m_size = sf::Vector2f(0.666 * sizeX, 0.666 * sizeY);
	m_sprite.scale(sf::Vector2f(m_size.x / (float)tex.getSize().x, m_size.y / (float)tex.getSize().y));

	sf::FloatRect bounds = m_sprite.getLocalBounds();
	m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	m_sprite.setRotation((180 * atan2(velocity.y, velocity.x) / PI) - 90);

}

bool Shot::getAllied()
{
	return m_allied;
}


void Shot::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	/*sf::FloatRect b = m_sprite.getGlobalBounds();
	sf::ConvexShape convex;
	convex.setPointCount(4);
	convex.setPoint(0, sf::Vector2f(b.left, b.top));
	convex.setPoint(1, sf::Vector2f(b.left + b.width, b.top));
	convex.setPoint(2, sf::Vector2f(b.left + b.width, b.top + b.height));
	convex.setPoint(3, sf::Vector2f(b.left, b.top + b.height));
	target.draw(convex, states);*/

	/*float radius = 0.85*(worldSizes["sun"].height) / 2.0;
	sf::CircleShape shape(radius);
	shape.setPosition(sf::Vector2f(worldMap["sun"].x - radius, worldMap["sun"].y - radius));
	shape.setFillColor(sf::Color(100, 250, 50));
	target.draw(shape, states);*/


	target.draw(m_sprite, states);
}



void Shot::updateCurrent(sf::Time dt)
{
	float deltatime = dt.asSeconds();
	float x = m_position.x += VELMOD * deltatime * m_velocity.x;
	float y = m_position.y += VELMOD * deltatime * m_velocity.y;

	setPosition(x, y);

	sf::FloatRect boundingRect = getWorldTransform().transformRect(m_sprite.getGlobalBounds());
	sf::FloatRect sunBRect = getWorldTransform().transformRect(worldSizes["sun"]);
	sf::FloatRect oppoBRect = getWorldTransform().transformRect(worldSizes["opponent"]);

	if (outOfBounds(getWorldPosition()))
	{
		setMarkedForRemoval(true);
	}
	//else if (boundingRect.intersects(sunBRect))
	//else if (m_sprite.getGlobalBounds().intersects(worldSizes["sun"]))
	/*else if (SceneNode::sunCollision(m_sprite.getPosition()))
	{
		setMarkedForRemoval(true);
	}
	//else if (boundingRect.intersects(oppoBRect))
	//else if (m_sprite.getGlobalBounds().intersects(worldSizes["opponent"]))
	else if (SceneNode::oppoCollision(m_sprite.getPosition()))
	{
		setMarkedForRemoval(true);
	}*/

}