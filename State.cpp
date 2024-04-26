#include "State.h"
#include "StateManager.h"

#include <iostream>

void State::render()
{

}

void State::update()
{

}

void State::init()
{

}

State::State(StateManager* _stateManager)
{
    this->stateManager = _stateManager;
}