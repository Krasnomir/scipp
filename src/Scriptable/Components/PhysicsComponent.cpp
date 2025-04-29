#include <iostream>

#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Util.hpp>

#include <Game.hpp>

namespace Scriptable::Components {
    PhysicsComponent::PhysicsComponent(RenderComponent* renderComponent) {
        m_renderComponent = renderComponent;
    }

    void PhysicsComponent::beforeRender(const EventData* data) {

        if(velocity.x == 0 && velocity.y == 0) return;

        float dt_seconds = data->deltaTime.asSeconds();

        sf::Vector2f previous_position = m_renderComponent->getPosition();
        sf::Vector2f new_position = sf::Vector2f(previous_position.x + (velocity.x * dt_seconds), previous_position.y + (velocity.y * dt_seconds));

        m_renderComponent->setPosition(new_position);

        // movement collission (prevent walking through certain entities)
        if(m_collidable) {
            auto* current_state = Scipp::globalGame->stateManager.currentState;
            auto collidable_entities = current_state->getEntitiesFromGroup("collidable");

            for(auto* entity : collidable_entities) {
                if(entity == parentEntity) continue;

                auto* entity_rc = entity->getComponent<Scriptable::Components::RenderComponent>();

                if(entity_rc->isColliding(m_renderComponent)) {
                    std::cout << "collission" << "\n";
                    m_renderComponent->setPosition(previous_position);
                    break;
                }
            }
        }
    }

    void PhysicsComponent::set_collidable(bool collidable) {
        auto* current_state = Scipp::globalGame->stateManager.currentState;
        auto* parent_entity = (Scriptable::Entity*) parentEntity;

        if(collidable && !m_collidable) {
            current_state->addEntityToGroup(parent_entity, "collidable");
            m_collidable = true;
            std::cout << "ADDED TO COLLIDABLE" << "\n";
        }
        else if(!collidable && collidable) {
            current_state->removeEntityFromGroup(parent_entity, "collidable");
            m_collidable = false;
        }
    }
}