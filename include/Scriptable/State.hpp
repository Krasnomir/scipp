#pragma once

#include <Camera.hpp>
#include <Scriptable/Entity.hpp>

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

		void initCamera();

		virtual void evokeAll(const std::string& eventName, const Scriptable::EventData* data);

		//returns 0 on fail
		Scriptable::Entity* getEntity(const std::string& entityName) noexcept;

		//returns true if an entity with specified name exists, false if not
		bool hasEntity(const std::string& entityName) const noexcept;

		//returns false on fail
		bool deleteComponent(const std::string& entityName);

		template<Scriptable::DerivedEntity T, class ... Args>
		inline bool addEntity(const std::string& entityName, Args ... args){
			// std::unique_lock<std::shared_mutex> writeLock(M_entityMapLock); temporary disabled
			
			if(M_entityMap.contains(entityName)) return false; // fails if there is already a entity with specified name

			T* newEntity = new T(args...);
			M_entityMap[entityName] = newEntity;

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


	private:
		mutable std::shared_mutex M_entityMapLock; // mutable means can be modified from a const function

		//every entity has a name (like in unity), only for internal use, please refer to the get/set methods for external use
		std::unordered_map<std::string, Scriptable::Entity*> M_entityMap;

		Camera M_RenderCamera;
	};
}