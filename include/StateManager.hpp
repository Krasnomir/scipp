#pragma once

#include <Scriptable/State.hpp>

class StateManager
{
	Scriptable::State* m_pendingState = nullptr;
public:
	Scriptable::State* currentState = nullptr;

	void changeState(Scriptable::State* state);
	void scheduleStateChange(Scriptable::State* state);
	void handleScheduledStateChanges();
	
	StateManager();
};

