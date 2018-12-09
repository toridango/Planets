#pragma once

#include "Planet.h"
#include "TextureHolder.h"
#include <limits>


constexpr auto DURATION = 5.0f; // Seconds
constexpr auto COOLDOWN = 2.0f; // Seconds
constexpr auto ANG_SPEED = 400.0f; // Seconds

class Shield
	: public Planet
{
public:
	explicit Shield(TextureHolder& textures, bool allied);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void updateCurrent(sf::Time dt);
	void setOrbitRadius(float r);
	void setActive(bool active, float angle);
	bool getActive() const;
	void setAngle(float angle);
	float getAngle() const;
	float getAngularWidth() const;

	void setAngularSpeed(float w);
	bool getAngularSpeed() const;
	float Shield::handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

	void synchAngularSpeed(float angSpeed, float angle, long ms_local, long ms_remote);
	/*virtual sf::FloatRect getGlobalBounds() const;
	bool collision(sf::Vector2f pos);*/

private:
	void tellRootAboutShields(bool active);

private:
	enum STATE{OFF, ON, CD};
	bool m_active;
	bool m_allied;
	bool m_needsCatchUp;
	STATE m_state;
	float m_angWidth;
	float m_realAngle;
	long m_newestTimeStamp;
	sf::Clock m_clock;

	sf::Sprite m_sprite;
	sf::Vector2f m_size;

};