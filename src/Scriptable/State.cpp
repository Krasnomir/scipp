#include <Scriptable/State.hpp>
#include <StateManager.hpp>

#include <Scriptable/Components/PhysicsComponent.hpp>

#include <iostream>
#include <limits>
#include <algorithm>

namespace Scriptable{
	bool compareZIndexes(const Entity* a, const Entity* b) {
		return a->zindex < b->zindex;
	}


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

			// maps cannot be sorted so we have to transfer it's data to a vector
			// the vector only contains entity pointers and no entity names because they aren't used in this context
			std::vector<Entity*> sortedEntities;
			for(auto& entity : M_entityMap) {
				sortedEntities.push_back(entity.second);
			}
			std::sort(sortedEntities.begin(), sortedEntities.end(), compareZIndexes);

			for (auto& entity : sortedEntities)
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

	void State::softDeleteUIObject(const std::string& objectName){
		std::unique_lock writeLock(M_uidelschdLock);
		M_delschd_uiArray.push_back(objectName);
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

		std::unique_lock writeLock3(M_uidelschdLock);
		for(auto & ui_schd_entry : M_delschd_uiArray){
			deleteUIObject(ui_schd_entry);
		}
		M_delschd_uiArray.clear();
	}

	void State::softDeleteEntity(const std::string& entityName){
		std::unique_lock<std::shared_mutex> writeLock(M_delschdLock);

		std::unique_lock<std::shared_mutex> writelock2(m_groupsLock);

		for(auto & [name, list] : m_groups){
			for(size_t i = 0; i < list.size(); i++){
				if(list[i]->getName() == entityName){
					list.erase(list.begin() + i);
					continue;
				}
			}
		}

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

	void State::addEntityToGroup(Entity* entity, std::string group) {
		std::unique_lock<std::shared_mutex> writeLock(m_groupsLock);

		m_groups[group].push_back(entity);
	}

	void State::removeEntityFromGroup(Entity* entity, std::string group) {
		std::unique_lock<std::shared_mutex> writeLock(m_groupsLock);
		std::string entityName = entity->getName();

		m_groups[group].erase(std::find(m_groups[group].begin(), m_groups[group].end(), entity));
	}

	// returns a nullptr when there are no entities in the specified group
	Entity* State::findClosestEntityFromGroup(Entity* finder, std::string group) {
		
		auto* finderRC = finder->getComponent<Scriptable::Components::RenderComponent>();

		float closestDistanceYet = std::numeric_limits<float>::max();
		Entity* closestEntityYet = nullptr;

		std::shared_lock<std::shared_mutex> readLock(m_groupsLock);
		std::shared_lock<std::shared_mutex> readLock1(M_entityMapLock);

		for(auto& entity : m_groups[group]) {
			auto* entityRC = entity->getComponent<Scriptable::Components::RenderComponent>();
			
			float distance = Util::getDistanceBetweenPoints(finderRC->getPosition(), entityRC->getPosition());

			if(distance < closestDistanceYet) {
				closestDistanceYet = distance;
				closestEntityYet = entity;
			}
		}

		return closestEntityYet;
	}

	std::vector<Entity*> State::getEntitiesFromGroup(std::string group) {
		std::shared_lock<std::shared_mutex> readLock(m_groupsLock);

		std::vector<Entity*> entities;
		for(auto& entity : m_groups[group]) {
			entities.push_back(entity);
		}

		return entities;
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