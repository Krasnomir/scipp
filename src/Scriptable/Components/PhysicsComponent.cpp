#include <iostream>

#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Util.hpp>

namespace Scriptable::Components {
    PhysicsComponent::PhysicsComponent(RenderComponent* renderComponent) {
        m_renderComponent = renderComponent;
    }

    void PhysicsComponent::beforeRender(const EventData* data) {
        // calculate the correct (fps independent) distance to move the entity using delta time and velocity
        float distance = velocity.magnitude * data->deltaTime.asMilliseconds();
        m_renderComponent->setPosition(Util::movePoint(m_renderComponent->getPosition(), velocity.magnitude, velocity.direction));
    }
}