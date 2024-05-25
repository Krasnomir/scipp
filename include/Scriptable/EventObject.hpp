#ifndef __event_object
#define __event_object 1

#include <string>
#include <unordered_map>
#include <functional>
#include <stdint.h>
#include <mutex>
#include <shared_mutex>
#include <SFML/Graphics.hpp>

namespace Scriptable{
    struct EventData {
        sf::RenderWindow* renderTarget;
    };

    class EventObject {
        public:

            EventObject();

            ~EventObject() = default;

            typedef std::function<void(const EventData*)> Event_t;

            
            virtual inline void beforeRender(const EventData* data) {}

            virtual inline void afterRender(const EventData* data) {}

            // adds callback to bound events
            void bindEvent(const std::string& eventName, Event_t callback);

            // call all bound events with the name of eventName
            void evokeEvents(const std::string& eventName, const EventData* data);

        protected:
            std::shared_mutex M_boundEventLock;

            //bound events
            std::unordered_map<std::string, std::vector<Event_t>> M_boundEvents;

        };
}

#endif