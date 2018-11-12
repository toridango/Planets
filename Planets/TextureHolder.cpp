
#include "TextureHolder.h"


void TextureHolder::load(Textures::ID id, const std::string& filename)
{
	std::unique_ptr<sf::Texture> texture(new sf::Texture());
	if (!texture->loadFromFile(filename))
		throw std::runtime_error("TextureHolder::load - Failed to load " + filename);

	auto inserted = m_texMap.insert(std::make_pair(id, std::move(texture)));

	assert(inserted.second);
}

sf::Texture& TextureHolder::get(Textures::ID id)
{
	auto found = m_texMap.find(id);
	return *found->second;
}

const sf::Texture& TextureHolder::get(Textures::ID id) const
{
	auto found = m_texMap.find(id);
	assert(found != m_texMap.end());

	return *found->second;
}

// TODO Generalizing the approach