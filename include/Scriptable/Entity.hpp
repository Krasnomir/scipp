#pragma once

#include <Scriptable/EventObject.hpp>
#include <Scriptable/Component.hpp>

#include <SFML/Graphics.hpp>

namespace Scriptable
{	
	class Entity : public EventObject
	{
	public:
		Entity();

		virtual ~Entity();

		template<DerivedComponent T>
		T* getComponent(){
			if(!hasComponent<T>()) return nullptr;

			std::shared_lock<std::shared_mutex> readLock(M_ComponentLock);

			for(auto * component : M_Components)
			{
				try
				{
					T* ptr = dynamic_cast<T*>(component); // this will throw an exception when component is derived from T
					if(ptr) return ptr;
				}
				catch(...) {}
			}

			return nullptr;
		}




		template<DerivedComponent T>
		bool deleteComponent()
		{
			if(!hasComponent<T>()) return false;
				
			std::unique_lock<std::shared_mutex> writeLock(M_delComponentLock);
				
			std::unique_lock<std::shared_mutex> writeLock1(M_ComponentLock);

			//cant iterate with an iterator because this loop removes a vector entry during iteration

			for(size_t i = 0; i < M_Components.size(); i++)
			{
				try{
					T* component = dynamic_cast<T>(M_Components[i]);
					
					M_components_tobe_deleted.push_back(component);
					M_Components.erase(M_Components.begin() + i);

					return true;
				}
				catch(...) {}
			}

			return false;

		}

		template<DerivedComponent T>
		inline bool hasComponent() const
		{
			std::shared_lock<std::shared_mutex> readLock(M_ComponentLock);

			for(auto * component : M_Components)
			{
				try
				{
					T* ptr = dynamic_cast<T*>(component); 
					if(ptr) return true;	
				}
				catch(...) {}
			}

			return false;
		} 
	

		//has to be declared in a header file because its a template 
		//creates an object of type T with no arguments
		template<DerivedComponent T>
		inline bool addComponent()
		{
			if(hasComponent<T>()) return false;

			std::unique_lock<std::shared_mutex> writeLock(M_ComponentLock);

			T* newComponent = new T();

			M_insertComponent_nolock(newComponent);

			return true;
		}

		//has to be declared in a header file because its a template 
		//creates an object of type T with Args... | T(Args...)
		template<DerivedComponent T, class ... Args>
		inline bool addComponent(const Args& ... args)
		{
			if(hasComponent<T>()) return false;

			std::unique_lock<std::shared_mutex> writeLock(M_ComponentLock);

			T* newComponent = new T(args...);

			M_insertComponent_nolock(newComponent);

			return true;
		}


		// calls all event with specified name from base class EventObject and all subcomponents
		void evokeAll(const std::string& eventName, const EventData* data);

		const std::string& getName() const;

		void exec_schd_deletion();

		friend class State;

	private:
		//Directly inserts component pointer into M_components, does not check for errors or race conditions (unsafe)
		void M_insertComponent_nolock(Component* component);

		mutable std::shared_mutex M_ComponentLock;

		std::vector<Component*> M_Components;

		mutable std::shared_mutex M_delComponentLock;
		std::vector<Component*> M_components_tobe_deleted;

		std::string M_name;
	};

	template<class T>
    concept DerivedEntity = std::is_base_of_v<Entity, T>;
}