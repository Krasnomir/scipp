#pragma once

#include "State.h"

class TestState : public State
{
	void init();
public:
	TestState(StateManager* stateManager);
};