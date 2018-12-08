#include "Shield.h"




Shield::Shield(TextureHolder& textures, bool allied)
	: m_sprite(textures.get(allied ? Textures::SHIELDPLAYER : Textures::SHIELDOPPO))
{
	float sizeX = 50.0f;
	float sizeY = 15.0f;
	//m_size = sf::Vector2f(60.0, 60.0);
	m_size = sf::Vector2f(0.666 * sizeX, 0.666 * sizeY);
	sf::Texture tex = textures.get(allied ? Textures::SHIELDPLAYER : Textures::SHIELDOPPO);
	m_sprite.scale(sf::Vector2f(m_size.x / (float)tex.getSize().x, m_size.y / (float)tex.getSize().y));

	sf::FloatRect bounds = m_sprite.getLocalBounds();
	m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
	
	m_angWidth = SH_ANG_WIDTH;
	m_angularSpeed = 0.0f;
	m_allied = allied;
	m_active = false;
	m_needsCatchUp = false;
	m_state = OFF;
}

void Shield::tellRootAboutShields(bool active)
{
	if (m_allied)
		SceneNode::playerShield = active;
	else
		SceneNode::oppoShield = active;
}

void Shield::setOrbitRadius(float r)
{
	float x = r * cos(m_angle / PI);
	float y = r * sin(m_angle / PI);
	m_orbitRadius = r;

	setPosition(x, y);
}

void Shield::setActive(bool active, float angle)
{

	if (active && m_state == OFF)
	{
		m_clock.restart();
		m_state = ON;
		m_active = active;
		m_angle = angle;
		tellRootAboutShields(active);
	}
	else if (!active)
	{
		m_active = active;
		tellRootAboutShields(active);
	}
}

bool Shield::getActive() const { return m_active; }
void Shield::setAngle(float angle) { m_angle = angle; }
bool Shield::getAngle() const { return m_angle; }
void Shield::setAngularSpeed(float w) { m_angularSpeed = w; }
bool Shield::getAngularSpeed() const { return m_angularSpeed; }
float Shield::getAngularWidth() const { return m_angWidth; }


float Shield::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (key == sf::Keyboard::A)
		m_angularSpeed = isPressed ? -ANG_SPEED : 0.0f;
	else if (key == sf::Keyboard::D)
		m_angularSpeed = isPressed ? ANG_SPEED : 0.0f;

	m_needsCatchUp = false;
	return m_angularSpeed;
}

void Shield::synchAngularSpeed(float angSpeed, long latency)
{
	m_angularSpeed = angSpeed;
	// Position prediction with latency into account
	float dt = ((float)(latency)) / 1000;
	float rAngle = m_angle + (m_angularSpeed - angSpeed) * dt;

	if (abs(rAngle - m_angle) > 1.0f)
	{
		m_realAngle = rAngle;
		m_needsCatchUp = true;
	}
}



void Shield::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_active)
		target.draw(m_sprite, states);
}



void Shield::updateCurrent(sf::Time dt)
{
	if (m_active)
	{
		float deltatime = dt.asSeconds();
		m_angle += deltatime * m_angularSpeed; // Angular speed needs to be 0 when neither A nor D are being pressed

		float catchUp = 0.0f;

		if (m_needsCatchUp)
		{
			m_realAngle += deltatime * m_angularSpeed;
			catchUp = m_realAngle - m_angle;

			m_angle += catchUp / 2.0f;
		}

		if (m_allied)
			SceneNode::playerShieldAngle = m_angle;
		else
			SceneNode::oppoShieldAngle = m_angle;

		setRotation(m_angle + 90);
		float x = m_orbitRadius * cos(m_angle * (PI / 180));
		float y = m_orbitRadius * sin(m_angle * (PI / 180));

		setPosition(x, y);
	}

	/*sf::Vector2f v = getParentPosition() - getWorldPosition(); // Is this how to fix the other collisions? it is
	setPosition(v.x, v.y);*/

	switch (m_state)
	{
	case ON:
		if (m_clock.getElapsedTime().asSeconds() > DURATION)
		{
			m_active = false;
			m_angularSpeed = 0.0f;
			tellRootAboutShields(m_active);
			m_state = CD;
		}
		break;
	case CD:
		if (m_clock.getElapsedTime().asSeconds() > (DURATION + COOLDOWN))
		{
			m_needsCatchUp = false;
			m_state = OFF;
		}
	case OFF:
	default:
		break;
	}

}

