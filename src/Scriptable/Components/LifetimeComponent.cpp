#include <Scriptable/Components/LifetimeComponent.hpp>

#include <iostream>

namespace Scriptable::Components {

    void LifetimeComponent::beforeRender(const EventData* data) {
        if(elapsed == false) {
            time -= data->deltaTime;

            if(time.asMicroseconds() < 0) {
                elapsed = true;
                onElapsedCallback();
            }
        }
    }

    void LifetimeComponent::extend(sf::Time t) {
        time += t;
    }

    void LifetimeComponent::restart(sf::Time t) {
        time = t;
        elapsed = false;
    }
}