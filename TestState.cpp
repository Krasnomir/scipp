#include "StateManager.h"
#include "State.h"
#include "TestState.h"

#include <iostream>

void TestState::init()
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

	this->components.push_back(triangle);
}

TestState::TestState(StateManager* stateManager) : State(stateManager)
{

}