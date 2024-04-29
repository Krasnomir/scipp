#include "StateManager.h"
#include "State.h"
#include "InitState.h"
#include "Entity.h"

#include <iostream>

void InitState::init()
{
	sf::VertexArray triangle(sf::TriangleStrip, 4);

	triangle[0].position = sf::Vector2f(0.f, 0.f);
	triangle[1].position = sf::Vector2f(0.f, 200.f);
	triangle[2].position = sf::Vector2f(200.f, 0.f);
	triangle[2].position = sf::Vector2f(200.f, 200.f);

	triangle[0].color = sf::Color::Yellow;
	triangle[1].color = sf::Color::Green;
	triangle[2].color = sf::Color::Red;
	triangle[3].color = sf::Color::Magenta;

	Entity entity(triangle);
	
	entity.setPosition(200.f, 200.f);
	entity.setRotation(30.f);

	this->components.push_back(entity);
}

void InitState::update(sf::Time elapsed)
{
	for (auto& entity : this->components)
	{
		int degPerSec = 45;
		entity.rotate(degPerSec * (elapsed.asMilliseconds() / 1000.f));
	}
}

InitState::InitState(StateManager* stateManager) : State(stateManager)
{

}