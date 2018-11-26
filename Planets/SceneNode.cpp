
#include "SceneNode.h"
std::map<std::string, sf::Vector2f> SceneNode::worldMap;
std::map<std::string, sf::FloatRect> SceneNode::worldSizes;



SceneNode::SceneNode()
	: m_children()
	, m_parent(nullptr)
	//, m_isMarkedForRemoval(false)
{

}

void SceneNode::attachChild(SNPtr child)
{
	child->m_parent = this;
	m_children.push_back(std::move(child));
}

SceneNode::SNPtr SceneNode::detachChild(const SceneNode& node)
{
	// lambda function is called on all the elements of children and will return true
	// when the pointer is the one that was asked for
	auto found = std::find_if(m_children.begin(), m_children.end(),
		[&](SNPtr& p) -> bool { return p.get() == &node; });

	// will be .end() if it wasn't found (end points to next vacancy, not last existing one)
	assert(found != m_children.end());

	// unbind from its parent
	SNPtr result = std::move(*found);
	result->m_parent = nullptr;
	m_children.erase(found);

	return result;
}

void SceneNode::update(sf::Time dt)
{
	updateCurrent(dt);
	updateChildren(dt);
}

void SceneNode::updateCurrent(sf::Time dt)
{
	// Do nothing by default
}

sf::FloatRect SceneNode::getGlobalBounds() const
{
	return sf::FloatRect();
}

void SceneNode::updateChildren(sf::Time dt)
{
	for (SNPtr& child : m_children)
	{
		child->update(dt);
		/*if (outOfBounds(child->getWorldPosition()))
		{
			child->setMarkedForRemoval(true);
		}
		else if (child->getGlobalBounds().intersects(worldSizes["sun"]))
		{
			child->setMarkedForRemoval(true);
		}
		else if (child->getGlobalBounds().intersects(worldSizes["opponent"]))
		{
			child->setMarkedForRemoval(true);
		}*/
	}
}


void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	drawCurrent(target, states);

	drawChildren(target, states);

}

void SceneNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Do nothing by default
}

void SceneNode::drawChildren(sf::RenderTarget& target, sf::RenderStates states) const
{
	// for (const SNPtr& child: m_children)
	for (auto itr = m_children.begin(); itr != m_children.end(); ++itr)
	{
		(*itr)->draw(target, states);
	}
}

sf::Vector2f SceneNode::getWorldPosition() const
{
	// Default constructor of vector2 creates a Vector2(0,0)
	return getWorldTransform() * sf::Vector2f();
}

sf::Transform SceneNode::getWorldTransform() const
{
	sf::Transform transform = sf::Transform::Identity;

	for (const SceneNode* node = this; node != nullptr; node = node->m_parent)
		transform = node->getTransform() * transform;

	return transform;
}

bool SceneNode::outOfBounds(sf::Vector2f pos)
{
	if (pos.x > WIN_WIDTH || pos.x < 0)
		return true;
	if (pos.y > WIN_HEIGHT || pos.y < 0)
		return true;
	return false;
}

void SceneNode::setMarkedForRemoval(bool b) 
{
	m_isMarkedForRemoval = b;
}

bool SceneNode::isMarkedForRemoval() const
{
	return m_isMarkedForRemoval;
}


void SceneNode::removeWrecks()
{
	int i = getChildrenCount();


	auto wreckfieldBegin = std::remove_if(m_children.begin(), m_children.end(), std::mem_fn(&SceneNode::isMarkedForRemoval));
	//auto wreckfieldBegin = std::remove_if(m_children.begin(), m_children.end(), [](auto const& pi) { return pi->isMarkedForRemoval(); });
	
	m_children.erase(wreckfieldBegin, m_children.end());

	std::for_each(m_children.begin(), m_children.end(), std::mem_fn(&SceneNode::removeWrecks));


	if (i != getChildrenCount())
	{
		int j = 0;
	}
}

// Auxiliary function to check if nodes were being removed
int SceneNode::getChildrenCount()
{
	int i = 0;
	for (SNPtr& child : m_children)
	{
		++i;
		/*if (child->isMarkedForRemoval()) ++i;
		else i += child->getChildrenCount();*/
	}
	return i;
}

bool SceneNode::sunCollision(sf::Vector2f pos)
{
	float distance = sqrt(pow(worldMap["sun"].x - pos.x, 2) + pow(worldMap["sun"].y - pos.y, 2));

	return distance <= 0.85*(worldSizes["sun"].height / 2);
}

bool SceneNode::oppoCollision(sf::Vector2f pos)
{
	float distance = sqrt(pow(worldMap["opponent"].x - pos.x, 2) + pow(worldMap["opponent"].y - pos.y, 2));

	return distance <= (worldSizes["opponent"].height / 2);
}