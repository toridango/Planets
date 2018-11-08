
#include "SceneNode.h"


SceneNode::SceneNode()
{
	m_parent = nullptr;
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
		[&] (SNPtr& p) -> bool { return p.get() == &node; });

	// will be .end() if it wasn't found (end points to next vacancy, not last existing one)
	assert(found != m_children.end());

	// unbind from its parent
	SNPtr result = std::move(*found);
	result->m_parent = nullptr;
	m_children.erase(found);

	return result;
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	drawCurrent(target, states);

	// for (const SNPtr& child: m_children)
	for (auto itr = m_children.begin(); itr != m_children.end(); ++itr)
	{
		(*itr)->draw(target, states);
	}
}

// TODO: pag61, continue creating types of entities and overwrite drawCurrent with their sprites