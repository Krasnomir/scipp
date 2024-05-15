#pragma once

#include "Game.h"
#include "State.h"

class StateManager
{
public:
	Game* game = nullptr;

	State* currentState = nullptr;

	void changeState(State* state);

	StateManager(Game* _game);

	
};

