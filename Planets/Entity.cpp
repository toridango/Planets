#include "Entity.h"



Entity::Entity()
{

}

void Entity::setVelocity(sf::Vector2f v)
{
	m_velocity = v;
}


void Entity::setVelocity(float vx, float vy)
{
	m_velocity.x = vx;
	m_velocity.y = vy;
}



sf::Vector2f Entity::getVelocity() const
{
	return m_velocity;
}

