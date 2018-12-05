#include "Planet.h"



Planet::Planet(Planets::Type type, TextureHolder& textures)
	: m_sprite(textures.get(toTextureID(type)))
{
	float sizeX = 50.0f;
	float sizeY = 50.0f;
	//m_size = sf::Vector2f(50.0, 50.0);
	m_size = sf::Vector2f(0.666 * sizeX, 0.666 * sizeY);
	sf::Texture tex = textures.get(toTextureID(type));
	m_sprite.scale(sf::Vector2f(m_size.x / (float)tex.getSize().x, m_size.y / (float)tex.getSize().y));

	sf::FloatRect bounds = m_sprite.getLocalBounds();
	m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	// Initialise physical parameters
	
	// angle (degrees)
	m_angle = 0.0f;

}



void Planet::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_sprite, states);
}


Textures::ID Planet::toTextureID(Planets::Type type)
{
	switch (type)
	{
	case Planets::Terran:
		return Textures::PLANETTERRAN;
	case Planets::City:
		return Textures::PLANETCITY;
	case Planets::Ice:
		return Textures::PLANETICE;
	case Planets::Lava:
		return Textures::PLANETLAVA;
	case Planets::Crosshairs:
		return Textures::CROSSHAIRS;
	}
	return Textures::SUN;
}


void Planet::updateCurrent(sf::Time dt)
{
	float deltatime = dt.asSeconds();
	m_angle += deltatime * m_angularSpeed;

	float x = m_orbitRadius * cos(m_angle / PI);
	float y = m_orbitRadius * sin(m_angle / PI);
	
	setPosition(x, y);

}

/*void Planet::move(float deltatime)
{
	m_angle += deltatime * m_angularSpeed;

	float x = m_orbitRadius * cos(m_angle / PI);
	float y = m_orbitRadius * sin(m_angle / PI);

	setPosition(x, y);
}*/

sf::FloatRect Planet::getGlobalBounds() const
{
	return m_sprite.getGlobalBounds();
}

void Planet::setOrbitRadius(float r)
{
	// Initialise angular speed
	// angular speed is proportional to sqrt of mass and inversely proportional to radius^3/2
	// the gravitational constant is of the order of 10^-11
	// but since the mass nor the distance are of the order of millions, I'm making up a constant too
	m_angularSpeed = G * sqrt(MASS / (pow(r, 3)));
		
	float x = r * cos(m_angle / PI);
	float y = r * sin(m_angle / PI);
	m_orbitRadius = r;

	setPosition(x, y);
}

// Latency in milliseconds, must divide to get seconds
void Planet::synchAngle(float a, sf::Int32 deltatime)
{
	setAngle(a + ((float)deltatime/1000) * m_angularSpeed);
}

void Planet::setAngle(float a)
{
	m_angle = a;
	float x = m_orbitRadius * cos(a / PI);
	float y = m_orbitRadius * sin(a / PI);

	setPosition(x, y);
}

float Planet::getAngle() const
{
	return m_angle;
}

bool Planet::collision(sf::Vector2f pos)
{
	float distance = sqrt(pow(m_sprite.getPosition().x - pos.x, 2) + pow(m_sprite.getPosition().y - pos.y, 2));

	return distance <= (m_sprite.getGlobalBounds().height / 2);
}
