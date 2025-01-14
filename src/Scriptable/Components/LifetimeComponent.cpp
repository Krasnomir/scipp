#include <Scriptable/Components/LifetimeComponent.hpp>

#include <iostream>

namespace Scriptable::Components {

    void LifetimeComponent::beforeRender(const EventData* data) {
        if(elapsed == false) {
            time -= data->deltaTime;
            std::cout << (float) time.asSeconds() << std::endl;

            if(time.asMicroseconds() < 0) {
                elapsed = true;
                this->onElapsedCallback();
            }
        }
    }

    void LifetimeComponent::extend(sf::Time t) {
        time += t;
    }
}