#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <assert.h>

#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>


class SceneNode
	: public sf::Transformable
	, public sf::Drawable
	, private sf::NonCopyable
{
public:
	typedef std::unique_ptr<SceneNode> SNPtr;

public:
	SceneNode();

	void attachChild(SNPtr child);
	SNPtr detachChild(const SceneNode& node);

	void update(sf::Time);

	sf::Vector2f getWorldPosition() const;
	sf::Transform getWorldTransform() const;

private:
	void updateCurrent(sf::Time);
	void updateChildren(sf::Time);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawChildren(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	std::vector<SNPtr> m_children;
	SceneNode* m_parent;
};