#pragma once

#include "Game.h"
#include "State.h"

class StateManager
{
	Game* game = nullptr;
public:
	State* currentState = nullptr;

	void changeState(State* state);

	StateManager(Game* _game);
};