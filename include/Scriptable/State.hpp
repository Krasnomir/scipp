#pragma once

#include <Camera.hpp>
#include <Scriptable/Entity.hpp>
#include <Scriptable/UI/UIObject.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <vector>

class StateManager;

namespace Scriptable{
	class State : public Scriptable::EventObject
	{
		
	public:
		State();

		virtual ~State();

		Camera M_camera;

		virtual void init();

		void initCamera();

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
			// std::unique_lock<std::shared_mutex> writeLock(M_entityMapLock); temporary disabled
			
			if(M_entityMap.contains(entityName)) return false; // fails if there is already a entity with specified name

			T* newEntity = new T(args...);
			M_entityMap[entityName] = newEntity;
			newEntity->M_name = entityName;

			return true;
		}

		template<Scriptable::DerivedEntity T>
		inline bool addEntity(const std::string& entityName){
			std::unique_lock<std::shared_mutex> writeLock(M_entityMapLock);
			
			if(M_entityMap.contains(entityName)) return false; // fails if there is already a entity with specified name

			T* newEntity = new T();
			M_entityMap[entityName] = newEntity;

			return true;
		}

		template<Scriptable::UI::DerivedUIObject T, class ... Args>
		inline bool addUIObject(const std::string& objectName, Args ... args){
			if(M_uiMap.contains(objectName)) return false;

			T* newObject = new T(args...);
			M_uiMap[objectName] = newObject;

			return true;
		}

		template<Scriptable::UI::DerivedUIObject T>
		inline bool addUIObject(const std::string& objectName){
			if(M_uiMap.contains(objectName)) return false;

			T* newObject = new T();
			M_uiMap[objectName] = newObject;

			return true;
		}


		bool hasUIObject(const std::string& objectName) const noexcept;

		//returns false on fail
		bool deleteUIObject(const std::string& objectName);

	private:
		mutable std::shared_mutex M_entityMapLock; // mutable means can be modified from a const function

		mutable std::shared_mutex M_uiMapLock; // mutable means can be modified from a const function

		std::unordered_map<std::string, Scriptable::UI::UIObject*> M_uiMap;

		//every entity has a name (like in unity), only for internal use, please refer to the get/set methods for external use
		std::unordered_map<std::string, Scriptable::Entity*> M_entityMap;

		mutable std::shared_mutex M_delschdLock;
		std::vector<std::string> M_delschd_entityArray;


		Camera M_RenderCamera;
	};
}