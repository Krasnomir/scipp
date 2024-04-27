#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

#include "Entity.h"

class StateManager;

class State
{
public:
	std::vector<Entity> components;

	StateManager* stateManager;

	virtual void render();
	virtual void update();
	virtual void init();

	State(StateManager* _stateManager);
};