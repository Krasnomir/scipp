#pragma once

#include <Camera.hpp>
#include <Scriptable/Entity.hpp>
#include <Scriptable/UI/Object.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <string>

class StateManager;

namespace Scriptable{
	class State : public Scriptable::EventObject
	{
		
	public:
		State();

		virtual ~State();

		void scheduleStateChange(State* state) {
			m_scheduledStateChange = true;
			m_scheduledState = state;
		}

		Camera M_camera;

		Camera getCamera();

		void addEntityToGroup(Entity* entity, std::string group);
		void removeEntityFromGroup(Entity* entity, std::string group);
		Entity* findClosestEntityFromGroup(Entity* finder, std::string group);
		std::vector<Entity*> getEntitiesFromGroup(std::string group);

		std::vector<Entity*> getEntities();

		virtual void init();

		void initCamera(sf::Vector2f windowSize);

		void evokeUIDraw(sf::RenderWindow* target);

		void evokeAll(const std::string& eventName, const Scriptable::EventData* data);

		//returns 0 on fail
		Scriptable::Entity* getEntity(const std::string& entityName) noexcept;

		//returns true if an entity with specified name exists, false if not
		bool hasEntity(const std::string& entityName) const noexcept;

		//returns false on fail
		bool deleteEntity(const std::string& entityName);

		void softDeleteEntity(const std::string& entityName);

		void exec_schd_deletion();

		template<Scriptable::DerivedEntity T, class ... Args>
		inline bool addEntity(const std::string& entityName, Args ... args){
			// std::unique_lock<std::shared_mutex> writeLock(M_entityMapLock);
			
			if(M_entityMap.contains(entityName)) return false; // fails if there is already a entity with specified name

			T* newEntity = new T(args...);
			M_entityMap[entityName] = newEntity;
			newEntity->setName(entityName);

			return true;
		}

		template<Scriptable::DerivedEntity T>
		inline bool addEntity(const std::string& entityName){
			std::unique_lock<std::shared_mutex> writeLock(M_entityMapLock);
			
			if(M_entityMap.contains(entityName)) return false; // fails if there is already a entity with specified name

			T* newEntity = new T();
			M_entityMap[entityName] = newEntity;
			newEntity->setName(entityName);

			return true;
		}

		template<Scriptable::UI::DerivedUIObject T, class ... Args>
		inline bool addUIObject(const std::string& objectName, Args ... args){
			std::shared_lock<std::shared_mutex> readLock(M_uiMapLock);
			if(M_uiMap.contains(objectName)) return false;

			T* newObject = new T(args...);
			M_uiMap[objectName] = newObject;

			return true;
		}

		template<Scriptable::UI::DerivedUIObject T>
		inline bool addUIObject(const std::string& objectName){
			// std::unique_lock<std::shared_mutex> writeLock(M_uiMapLock);

			if(M_uiMap.contains(objectName)) return false;

			T* newObject = new T();
			M_uiMap[objectName] = newObject;

			return true;
		}

		UI::Object* getUIObject(const std::string& objectName);

		bool hasUIObject(const std::string& objectName) const noexcept;

		//returns false on fail
		bool deleteUIObject(const std::string& objectName);

		void softDeleteUIObject(const std::string& objectName);

		std::vector<UI::Object*> getObjectsByGroup(const std::string& groupName);

		void addSound(std::string path, std::string name);
		void removeSound(std::string name);
		void playSound(std::string name);

protected: 

		bool m_scheduledStateChange = false;
		State* m_scheduledState = nullptr;
		
private:

		mutable std::shared_mutex m_soundsLock;
		std::map<std::string, sf::Sound> m_sounds;

		mutable std::shared_mutex M_entityMapLock; // mutable means can be modified from a const function
		//every entity has a name (like in unity), only for internal use, please refer to the get/set methods for external use
		std::unordered_map<std::string, Scriptable::Entity*> M_entityMap;

		mutable std::shared_mutex M_uiMapLock; // mutable means can be modified from a const function
		std::unordered_map<std::string, Scriptable::UI::Object*> M_uiMap;

		mutable std::shared_mutex M_delschdLock;
		std::vector<std::string> M_delschd_entityArray;

		mutable std::shared_mutex M_uidelschdLock;
		std::vector<std::string> M_delschd_uiArray;

		mutable std::shared_mutex m_groupsLock;
		std::map<std::string, std::vector<Scriptable::Entity*>> m_groups;

		Camera M_RenderCamera;
	};
}