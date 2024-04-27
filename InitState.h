#pragma once

#include "State.h"

class InitState : public State
{
	void init();
public:
	InitState(StateManager* stateManager);
};