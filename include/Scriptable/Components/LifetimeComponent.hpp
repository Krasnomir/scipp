#ifndef __LIFETIME_COMPONENT
#define __LIFETIME_COMPONENT 1

#include <functional>

#include <Scriptable/Component.hpp>

namespace Scriptable::Components {

    class LifetimeComponent : public Component {
        std::function<void(LifetimeComponent*)> onElapsedCallback;
        sf::Time time;
        bool elapsed = false;
    public:
        LifetimeComponent() = delete;
        LifetimeComponent(sf::Time t, std::function<void(LifetimeComponent*)> onElapsed) : time(t), onElapsedCallback(onElapsed) {};
        virtual ~LifetimeComponent() = default;

        // extends or cuts (if the paramter is negative) the lifetime
        void extend(sf::Time t);
        void restart(sf::Time t);

        void beforeRender(const EventData* data);
    };
}

#endif