#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

class StateManager;

class State
{
protected:
	std::vector<sf::VertexArray> components;
public:
	StateManager* stateManager;

	virtual void render();
	virtual void update();
	virtual void init();

	State(StateManager* _stateManager);
};