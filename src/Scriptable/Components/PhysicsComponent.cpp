#include <iostream>

#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Util.hpp>

#include <Game.hpp>

namespace Scriptable::Components {
    PhysicsComponent::PhysicsComponent(RenderComponent* renderComponent) {
        m_renderComponent = renderComponent;
    }

    void PhysicsComponent::beforeRender(const EventData* data) {
        float dt_seconds = data->deltaTime.asSeconds();

        sf::Vector2f previous_position = m_renderComponent->getPosition();
        sf::Vector2f new_position = sf::Vector2f(previous_position.x + (velocity.x * dt_seconds), previous_position.y + (velocity.y * dt_seconds));

        m_renderComponent->setPosition(new_position);

        /*
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
                            m_renderComponent->setPosition(previous_position);
                        }
                    }
                }
            }
        }
        */
    }
}