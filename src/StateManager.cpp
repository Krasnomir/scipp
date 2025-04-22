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

void StateManager::scheduleStateChange(Scriptable::State* state) {
	m_pendingState = state;
}

void StateManager::handleScheduledStateChanges() {
	if(m_pendingState) {
		if (currentState) {
			delete currentState;
		}

        currentState = m_pendingState;
        currentState->init();
        m_pendingState = nullptr;
	}
}

StateManager::StateManager()
{

}