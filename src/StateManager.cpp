#include <Game.hpp>
#include <StateManager.hpp>
#include <Scriptable/State.hpp>

void StateManager::changeState(Scriptable::State* state)
{
	// Jesli byl poprzedni stan, to go usun
	if (this->currentState != 0)
	{
		delete this->currentState;
	}

	this->currentState = state;

}

StateManager::StateManager()
{

}