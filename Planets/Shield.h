#pragma once

#include "Entity.h"
#include "TextureHolder.h"


constexpr auto DURATION = 1.0f; // Seconds
constexpr auto COOLDOWN = 2.0f; // Seconds

class Shield
	: public Entity
{
public:
	explicit Shield(TextureHolder& textures, bool allied);
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void updateCurrent(sf::Time dt) override;
	void setActive(bool active);
	bool getActive() const;
	/*virtual sf::FloatRect getGlobalBounds() const;
	bool collision(sf::Vector2f pos);*/

private:
	void tellRootAboutShields(bool active);

private:
	enum STATE{OFF, ON, CD};
	bool m_active;
	bool m_allied;
	STATE m_state;
	sf::Clock m_clock;

	sf::Sprite m_sprite;
	sf::Vector2f m_size;

};