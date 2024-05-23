#pragma once

#include <SFML/Graphics.hpp>
#include <Scriptable/EventObject.hpp>
#include <Scriptable/Component.hpp>

namespace Scriptable{



	class Entity : public EventObject
	{
		public:
			Entity();

			~Entity();


			template<DerivedComponent T>
			bool deleteComponent()
			{
				if(!hasComponent<T>()) return false;
				
				std::unique_lock<std::shared_mutex> writeLock(M_ComponentLock);
				
				//cant iterate with an iterator because this loop removes a vector entry during iteration

				for(size_t i = 0; i < M_Components.size(); i++)
				{
					try{
						T* component = dynamic_cast<T>(M_Components[i]);
						delete component;
						M_Components.erase(M_Components.begin() + i);

						return true;
					}
					catch(...) {}
				}

				return false;

			}

			template<DerivedComponent T>
			inline bool hasComponent()
			{
				std::shared_lock<std::shared_mutex> readLock(M_ComponentLock);

				for(auto * component : M_Components)
				{
					try
					{
						dynamic_cast<T*>(component); // this will throw an exception when component is derived from T

						return true;
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

		private:
			//Directly inserts component pointer into M_components, does not check for errors or race conditions (unsafe)
			void M_insertComponent_nolock(Component* component);

			std::shared_mutex M_ComponentLock;

			std::vector<Component*> M_Components;
		
	};


}