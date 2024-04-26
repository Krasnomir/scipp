#include "StateManager.h"
#include "State.h"
#include "TestState.h"

#include <iostream>

void TestState::init()
{
	
}

TestState::TestState(StateManager* stateManager) : State(stateManager)
{

}