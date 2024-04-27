#pragma once

#include <SFML/Graphics.hpp>

class Entity : public sf::Drawable, public sf::Transformable
{
	sf::VertexArray vertices;
	sf::Texture texture;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	Entity(sf::VertexArray _vertices);
};

