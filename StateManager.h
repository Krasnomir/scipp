#pragma once

#include "State.h"

class StateManager
{
public:
	State* currentState = nullptr;

	void changeState(State* state);

	StateManager();
};

