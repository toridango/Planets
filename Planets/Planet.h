#pragma once

#include <cmath>

#include "Entity.h"
#include "TextureHolder.h"

constexpr auto PI = 3.14159265358979323846;
constexpr auto G = 36.0;
constexpr auto MASS = 200000.0;

namespace Planets
{
	enum Type { Terran, City, Ice, Lava, Crosshairs };
};


class Planet
	: public Entity
{
public:
	explicit Planet(Planets::Type type, TextureHolder& textures);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void setRadius(float r);
	void setAngle(float a);
	Textures::ID toTextureID(Planets::Type type);
	void move(float dt);

protected:
	sf::Sprite m_sprite;
	sf::Vector2f m_size;
	float m_orbitRadius;
	float m_angularSpeed;
	float m_angle; // degrees to keep it simple

};