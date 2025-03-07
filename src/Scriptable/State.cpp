#include <Scriptable/State.hpp>
#include <StateManager.hpp>

#include <iostream>

namespace Scriptable{
	void State::evokeAll(const std::string& eventName, const Scriptable::EventData* data)
	{
		evokeEvents(eventName, data); // evoke all events from base eventObject class

			std::shared_lock<std::shared_mutex> readLock(M_uiMapLock);

			for (auto& [name, uiobj] : M_uiMap)
			{
				if(eventName != "onRender")
					uiobj->evokeEvents(eventName, data);
			}

		{
			std::shared_lock<std::shared_mutex> readLock(M_entityMapLock);

			for (auto& [name, entity] : M_entityMap)
			{
				entity->evokeAll(eventName, data);
			}
		}
	}

	void State::evokeUIDraw(sf::RenderWindow* target){
		std::shared_lock<std::shared_mutex> readLock(M_uiMapLock);
		EventData data;
		data.currentState = this;
		data.targetWindow = target;

		for (auto& [name, uiobj] : M_uiMap)
		{
			uiobj->draw_to_screen(&data);
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

	bool State::hasUIObject(const std::string& objectName) const noexcept{
		std::shared_lock<std::shared_mutex> readLock(M_uiMapLock);

		for (auto& [name, entity] : M_uiMap)
		{
			if(name == objectName) return true;
		}

		return false;
	}


	bool State::deleteUIObject(const std::string& objectName){
		std::unique_lock<std::shared_mutex> writeLock(M_uiMapLock);

		if(!M_uiMap.contains(objectName)) return false;

		delete M_uiMap[objectName];

		M_uiMap.erase(objectName);

		return true;
	}

	bool State::deleteEntity(const std::string& entityName){
		std::unique_lock<std::shared_mutex> writeLock(M_entityMapLock);

		if(!M_entityMap.contains(entityName)) return false;

		delete M_entityMap[entityName];

		M_entityMap.erase(entityName);

		return true;
	}

	void State::exec_schd_deletion(){
		std::unique_lock<std::shared_mutex> writeLock(M_delschdLock);
		
		{
			std::unique_lock<std::shared_mutex> writeLock2(M_entityMapLock);
		
			for(auto& [name, ptr] : M_entityMap){
				ptr->exec_schd_deletion();
			}
		}
		
		for(auto & schd_entry : M_delschd_entityArray){
			deleteEntity(schd_entry);
		}

		M_delschd_entityArray.clear();
	}

	void State::softDeleteEntity(const std::string& entityName){
		std::unique_lock<std::shared_mutex> writeLock(M_delschdLock);

		M_delschd_entityArray.push_back(entityName);
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