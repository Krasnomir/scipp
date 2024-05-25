#include "State.h"
#include "StateManager.h"

#include <iostream>

void State::evokeAll(const std::string& eventName, const Scriptable::EventData* data)
{
	std::unique_lock<std::shared_mutex> writeLock(M_entityMapLock);

	for (auto& [name, entity] : M_entityMap)
	{
		entity->evokeEvents(eventName, data);
	}
}

void State::render()
{

}

void State::update(sf::Time elapsed)
{

}


void State::init()
{

}

State::State()
{

}