#include <Scriptable/Entity.hpp>

#include <SFML/Graphics.hpp>

namespace Scriptable{
	Entity::Entity(){}

    Entity::~Entity()
    {
        std::unique_lock<std::shared_mutex> writeLock(M_ComponentLock);

        //destroy all components
        for(auto * component : M_Components){
            delete component;
        }
    }
    
    void Entity::evokeAll(const std::string& eventName, const EventData* data){
        evokeEvents(eventName, data); //evoke events from base class

        std::shared_lock<std::shared_mutex> readLock(M_ComponentLock);

        for(auto * component : M_Components){
            component->evokeEvents(eventName, data);
        }
    }

    void Entity::M_insertComponent_nolock(Component* component)
    {
        M_Components.push_back(component);
    }
}