#include "Button.h"



Button::Button()
	: m_sprite()
	, m_font()
	, m_text()
{
	if (!m_font.loadFromFile("Media/Fonts/bahnschrift.ttf"))
	{
		throw "Font didn't load!";
	}

	if (!m_texture.loadFromFile("Media/button.png"))
	{
		throw "Image didn't load!";
	}

	m_sprite.setTexture(m_texture);
	m_text.setFont(m_font);
	//m_text.setFillColor(sf::Color(61, 202, 226, 255));
	m_text.setFillColor(sf::Color(29, 91, 122, 255));
}

Button::Button(BTYPE b)
	: Button()
{
	type = b;
}


void Button::centreText(sf::Vector2f spritePos)
{
	sf::Vector2u t_pos(spritePos.x + m_texture.getSize().x / 2 - m_text.getLocalBounds().width / 2.0, spritePos.y + m_texture.getSize().y / 2 - m_text.getLocalBounds().height / 1.5);
	m_text.setPosition(t_pos.x, t_pos.y);
}

void Button::setText(std::string text)
{
	m_text.setString(text);
	m_text.setFont(m_font);
}

void Button::setSize(sf::Vector2f size)
{
	m_buttonSize = size;
	m_texSize = m_texture.getSize();
	m_sprite.scale(sf::Vector2f(m_buttonSize.x / (float)m_texSize.x, m_buttonSize.y / (float)m_texSize.y));

	flag_size = true;
}


void Button::setPosition(sf::Vector2f pos)
{
	m_sprite.setPosition(pos);
	centreText(pos);
}


void Button::setType(BTYPE bt)
{
	type = bt;
}


sf::Sprite Button::getSprite()
{
	return m_sprite;
}

sf::Text Button::getText()
{
	return m_text;
}


bool Button::isMouseOverlap(sf::Vector2i mousepos)
{
	return m_sprite.getGlobalBounds().contains(mousepos.x, mousepos.y);
}

void Button::highlight(bool flag)
{
	if (flag)
	{
		m_text.setStyle(sf::Text::Underlined | sf::Text::Bold);
		centreText(m_sprite.getPosition());
	}
	else
	{
		m_text.setStyle(sf::Text::Regular);
		centreText(m_sprite.getPosition());
	}
}

BTYPE Button::getBType()
{
	return type;
}