#include "StateManager.h"
#include "State.h"
#include "InitState.h"
#include "Entity.h"

#include <SFML/Graphics.hpp>

#include <iostream>

void InitState::addTriangle(sf::Vector2f pos)
{
	sf::VertexArray triangle(sf::TriangleStrip, 3);

	triangle[0].position = sf::Vector2f(0.f, 0.f);
	triangle[1].position = sf::Vector2f(0.f, 100.f);
	triangle[2].position = sf::Vector2f(100.f, 0.f);

	triangle[0].color = sf::Color::Red;
	triangle[1].color = sf::Color::Yellow;
	triangle[2].color = sf::Color::Green;

	Entity entity(triangle);

	entity.setPosition(pos.x, pos.y);
	entity.setRotation(30.f);

	entity.setOrigin(50.f, 50.f);

	this->components.push_back(entity);
}

void InitState::init()
{
	int rate = 50;

	for (int i = 0; i < 800; i += rate)
	{
		for (int j = 0; j < 600; j += rate)
		{
			this->addTriangle(sf::Vector2f(i, j));
		}
	}
}

void InitState::update(sf::Time elapsed)
{
	int degPerSec = 180;

	for (auto& entity : this->components)
	{
		entity.rotate(degPerSec * (elapsed.asMilliseconds() / 1000.f));
	}
}

InitState::InitState(StateManager* stateManager) : State(stateManager)
{

}