#include <SFML/Graphics.hpp>

#include "Entity.h"

void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= this->getTransform();
	states.texture = &this->texture;

	target.draw(this->vertices, states);
}

Entity::Entity(sf::VertexArray _vertices)
{
	this->vertices = _vertices;
}