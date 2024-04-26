#include "Game.h"
#include "StateManager.h"
#include "State.h"

void StateManager::changeState(State* state)
{
	// Jesli by³ poprzedni stan, to go usun¹æ
	if (this->currentState != 0)
	{
		delete this->currentState;
	}

	this->currentState = state;
	this->currentState->init();
}

StateManager::StateManager(Game *_game)
{
	this->game = _game;
}