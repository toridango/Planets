#pragma once


#include "Entity.h"
#include "TextureHolder.h"

//constexpr auto PI = 3.14159265358979323846;
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
	void setOrbitRadius(float r);
	void setAngle(float a);
	Textures::ID toTextureID(Planets::Type type);
	void updateCurrent(sf::Time dt) override;
	//void move(float dt);
	virtual sf::FloatRect getGlobalBounds() const;
	bool collision(sf::Vector2f pos);

protected:
	sf::Sprite m_sprite;
	sf::Vector2f m_size;
	float m_orbitRadius;
	float m_angularSpeed;
	float m_radius;
	float m_angle; // degrees to keep it simple

};