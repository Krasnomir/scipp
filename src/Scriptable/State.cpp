#include <Scriptable/State.hpp>
#include <StateManager.hpp>

#include <iostream>

namespace Scriptable{
	void State::evokeAll(const std::string& eventName, const Scriptable::EventData* data)
	{
		evokeEvents(eventName, data); // evoke all events from base eventObject class

		std::shared_lock<std::shared_mutex> readLock(M_entityMapLock);

		for (auto& [name, entity] : M_entityMap)
		{
			entity->evokeAll(eventName, data);
		}
	}

	bool State::hasEntity(const std::string& entityName) const noexcept{
		std::shared_lock<std::shared_mutex> readLock(M_entityMapLock);

		for (auto& [name, entity] : M_entityMap)
		{
			if(name == entityName) return true;
		}

		return false;
	}

	bool State::deleteEntity(const std::string& entityName){
		std::unique_lock<std::shared_mutex> writeLock(M_entityMapLock);

		if(!M_entityMap.contains(entityName)) return false;

		delete M_entityMap[entityName];

		M_entityMap.erase(entityName);

		return true;
	}

	Scriptable::Entity* State::getEntity(const std::string& entityName) noexcept{
		std::shared_lock<std::shared_mutex> readLock(M_entityMapLock);

		for (auto& [name, entity] : M_entityMap)
		{
			if(name == entityName) return entity;
		}

		return nullptr;
	}

	void State::init()
	{
		
	}

	void State::initCamera()
	{
		M_camera = Camera(sf::Vector2f(800, 600), sf::Vector2f(0, 0));
		M_camera.apply();
	}

	State::~State()
	{
		std::unique_lock<std::shared_mutex> entityWriteLock(M_entityMapLock);
		
		for(auto& [name, entity] : M_entityMap)
		{
			delete entity;
		}

		M_entityMap.clear();

	}

	State::State() {}
}