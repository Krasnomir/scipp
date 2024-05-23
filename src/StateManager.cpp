#include "Game.h"
#include "StateManager.h"
#include "State.h"

void StateManager::changeState(State* state)
{
	// Jesli byl poprzedni stan, to go usun
	if (this->currentState != 0)
	{
		delete this->currentState;
	}

	this->currentState = state;
	this->currentState->init();
}

StateManager::StateManager()
{

}