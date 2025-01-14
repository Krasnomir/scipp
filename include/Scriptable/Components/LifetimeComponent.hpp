#ifndef __LIFETIME_COMPONENT_HPP
#define __LIFETIME_COMPONENT

#include <functional>

#include <Scriptable/Component.hpp>
#include <Scriptable/Entity.hpp>

namespace Scriptable::Components {

    class LifetimeComponent : public Component {
        std::function<void()> onElapsedCallback;
        sf::Time time;
        bool elapsed = false;
    public:
        LifetimeComponent() = delete;
        LifetimeComponent(sf::Time t, std::function<void()> onElapsed) : time(t), onElapsedCallback(onElapsed) {};
        virtual ~LifetimeComponent() = default;

        // extends or cuts (if the paramter is negative) the lifetime
        void extend(sf::Time t);
        void restart(sf::Time t);

        void beforeRender(const EventData* data);
    };
}

#endif