#ifndef __event_object
#define __event_object 1

#include <SFML/Graphics.hpp>

#include <string>
#include <unordered_map>
#include <functional>
#include <stdint.h>
#include <mutex>
#include <shared_mutex>

namespace Scriptable{
    struct EventData {
        //time since last frame
        sf::Time deltaTime;
        sf::Event sfmlEvent;

        sf::RenderWindow* targetWindow;
        class State* currentState;
    };

    class EventObject {
    public:
        EventObject();

        explicit operator sf::ConvexShape() = delete;

        virtual ~EventObject() = default;

        typedef std::function<void(const EventData*)> Event_t;
            
        virtual inline void beforeRender(const EventData* data) {}

        virtual inline void onRender(const EventData* data) {}

        virtual inline void onWindowClosed(const EventData* data) {}

        virtual inline void onMouseMoved(const EventData* data) {}

        virtual inline void onMouseButtonPressed(const EventData* data) {}

        virtual inline void onMouseButtonReleased(const EventData* data) {}

        virtual inline void onKeyPressed(const EventData* data) {}

        virtual inline void onKeyReleased(const EventData* data) {}

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