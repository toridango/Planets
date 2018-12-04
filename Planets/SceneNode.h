#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <assert.h>
#include <map>
#include <functional>

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

constexpr auto WIN_WIDTH = (int)(2.0 / 3.0 * 1580);
constexpr auto WIN_HEIGHT = (int)(2.0 / 3.0 * 1000);

class SceneNode
	: public sf::Transformable
	, public sf::Drawable
	, private sf::NonCopyable
{
public:
	typedef std::unique_ptr<SceneNode> SNPtr;
	// Positions map where sun and planets will be stored and updated
	// value is in order: position, width&height
	static std::map<std::string, sf::Vector2f> worldMap;
	static std::map<std::string, sf::FloatRect> worldSizes;

public:
	SceneNode();

	void attachChild(SNPtr child);
	SNPtr detachChild(const SceneNode& node);

	void update(sf::Time);

	sf::Vector2f getWorldPosition() const;
	sf::Transform getWorldTransform() const;

	bool outOfBounds(sf::Vector2f pos);
	void setMarkedForRemoval(bool);
	bool isMarkedForRemoval() const;
	void removeWrecks();
	int getChildrenCount();
	virtual sf::FloatRect getGlobalBounds() const;
	static bool sunCollision(sf::Vector2f pos);
	static bool oppoCollision(sf::Vector2f pos);
	sf::Vector2f getParentPosition() const;

private:
	virtual void updateCurrent(sf::Time);
	void updateChildren(sf::Time);
	sf::FloatRect getBoundingRect() const;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawBoundingRect(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<SNPtr> m_children;
	SceneNode* m_parent;
	bool m_isMarkedForRemoval = false;
};