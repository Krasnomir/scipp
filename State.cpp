#include "State.h"
#include "StateManager.h"

#include <iostream>

void State::render()
{

}

void State::update(sf::Time elapsed)
{

}

void State::init()
{

}

State::State(StateManager* _stateManager)
{
    this->stateManager = _stateManager;
}