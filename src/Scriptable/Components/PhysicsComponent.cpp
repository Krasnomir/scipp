#include <iostream>

#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Util.hpp>

#include <Game.hpp>

namespace Scriptable::Components {
    PhysicsComponent::PhysicsComponent(RenderComponent* renderComponent) {
        m_renderComponent = renderComponent;
    }

    void PhysicsComponent::beforeRender(const EventData* data) {
        sf::Vector2f previousPosition = m_renderComponent->getPosition();

        // calculate the correct (fps independent) distance to move the entity using delta time and velocity
        float distance = velocity.magnitude * data->deltaTime.asMilliseconds();
        m_renderComponent->setPosition(Util::movePoint(m_renderComponent->getPosition(), velocity.magnitude, velocity.direction));

        if(collidable) {
            auto collidableEntities = Scipp::globalGame->stateManager.currentState->getEntitiesFromGroup("collidable");

            auto entity = (Entity*)parentEntity;
            auto* rc = entity->getComponent<Scriptable::Components::RenderComponent>();

            auto entities = Scipp::globalGame->stateManager.currentState->getEntities();

            for(auto& entity : entities) {
                if(entity->hasComponent<Scriptable::Components::PhysicsComponent>() && entity != parentEntity) {
                    auto* entity_pc = entity->getComponent<Scriptable::Components::PhysicsComponent>();
                    auto* entity_rc = entity->getComponent<Scriptable::Components::RenderComponent>();

                    if(entity_pc->collidable) {
                        if(entity_rc->isColliding(rc)) {
                            std::cout << "COLLISSION" << "\n";
                            m_renderComponent->setPosition(previousPosition);
                        }
                    }
                }
            }
        }
    }
}