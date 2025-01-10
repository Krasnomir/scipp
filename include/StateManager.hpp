#pragma once

#include <Scriptable/State.hpp>

class StateManager
{
public:
	Scriptable::State* currentState = nullptr;

	void changeState(Scriptable::State* state);
	
	StateManager();
};

