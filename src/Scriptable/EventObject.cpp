#include <Scriptable/EventObject.hpp>

namespace Scriptable{
    EventObject::EventObject()
    {
        //binds the virtual onUpdate method
        bindEvent("beforeRender", std::bind(&EventObject::beforeRender, this, std::placeholders::_1));
        bindEvent("afterRender", std::bind(&EventObject::afterRender, this, std::placeholders::_1));
    }

    void EventObject::bindEvent(const std::string& eventName, Event_t callback)
    {   
        //establish a full lock on the bound events map (no other thread can read or write)
        std::unique_lock<std::shared_mutex> writeLock(M_boundEventLock);
        
        M_boundEvents[eventName].push_back(callback);
    }

    void EventObject::evokeEvents(const std::string& eventName, const EventData* data)
    {
        //establish a read only lock on the bound events map (other threads can only read)
        std::shared_lock<std::shared_mutex> readLock(M_boundEventLock);

        if(M_boundEvents.contains(eventName)){
            for(auto & event : M_boundEvents[eventName]){
                event(data);
            }
        }
    }
}