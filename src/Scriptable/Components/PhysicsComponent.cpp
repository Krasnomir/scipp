#include <iostream>

#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Util.hpp>

namespace Scriptable::Components {
    PhysicsComponent::PhysicsComponent(float b, Scriptable::Components::RenderComponent* rc) {
        speed = b;
        velocity = b;
        renderComponent = rc;
    }

    void PhysicsComponent::beforeRender(const EventData* data) {
        // calculate the correct (fps independent) distance to move the entity using delta time and velocity
        float distance = this->velocity * data->deltaTime.asMilliseconds();
        renderComponent->setPosition(Util::movePoint(renderComponent->getPosition(), this->velocity, renderComponent->getRotation()));
    }
}