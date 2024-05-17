#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <vector>

#include "Entity.h"

class StateManager;

class State
{
public:
	std::vector<Entity> components;

	sf::View viewPort;

	virtual void render();
	virtual void update(sf::Time elapsed);
	virtual void init();

	State();
};