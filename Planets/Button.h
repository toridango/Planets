#pragma once

#include <iostream>

#include <SFML/Graphics.hpp>

enum BTYPE { BHOST, BJOIN, EXIT, NONE };

class Button
{
public:
	Button();
	Button(BTYPE);

	void setText(std::string text);
	void setSize(sf::Vector2f size);
	void setPosition(sf::Vector2f pos);
	void setType(BTYPE);


	sf::Sprite getSprite();
	sf::Text getText();
	BTYPE getBType();
	bool isMouseOverlap(sf::Vector2i mousepos);
	void highlight(bool flag);
	void setLastButton();
	static BTYPE getLastButton();
	static BTYPE Button::lastButton;


private:

	bool flag_size = false;

	sf::Font m_font;
	sf::Text m_text;

	sf::Texture m_texture;
	sf::Sprite m_sprite;

	sf::Vector2f m_pos;
	sf::Vector2u m_texSize;
	sf::Vector2f m_buttonSize;

	BTYPE type;

	void centreText(sf::Vector2f spritePos);

};