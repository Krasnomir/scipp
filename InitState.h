#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "State.h"

class InitState : public State
{
	void init();
	void update(sf::Time elapsed);
public:
	InitState(StateManager* stateManager);
};