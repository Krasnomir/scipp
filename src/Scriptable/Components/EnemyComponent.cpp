#include <Scriptable/Components/EnemyComponent.hpp>
#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Scriptable/Components/HealthComponent.hpp>
#include <Game.hpp>

#include <iostream>

namespace Scriptable::Components {
    void EnemyComponent::beforeRender(const EventData* data) {
        // rotates the entity so that it faces the closest target
        // when it's close enough to the target and the cooldown has passed it starts dealing damage
        Entity* parentEntity = (Entity*) this->parentEntity; // cast parentEntity generic pointer to an Entity pointer
        m_target = Scipp::globalGame->stateManager.currentState->findClosestEntityFromGroup(parentEntity, "friendly");

        if(m_target != nullptr) {

            auto* entity_rc = parentEntity->getComponent<Scriptable::Components::RenderComponent>();
            auto* entity_pc = parentEntity->getComponent<Scriptable::Components::PhysicsComponent>();
            auto* target_rc = m_target->getComponent<Scriptable::Components::RenderComponent>();

            entity_rc->setRotation(Util::getAngleBetweenPoints(entity_rc->getPosition(), target_rc->getPosition()));
            entity_pc->velocity.direction = Util::getAngleBetweenPoints(entity_rc->getPosition(), target_rc->getPosition());

            if(Util::getDistanceBetweenPoints(entity_rc->getPosition(), target_rc->getPosition()) <= reach) {
                entity_pc->velocity.magnitude = 0;

                if(m_cooldownTracker <= sf::seconds(0)) {
                    m_cooldownTracker = m_cooldown;

                    auto* target_hc = m_target->getComponent<Scriptable::Components::HealthComponent>();
                    target_hc->setHealth(target_hc->getHealth() - damage);
                }
                else {
                    m_cooldownTracker -= data->deltaTime;
                }
            }
            else {
                entity_pc->velocity.magnitude = m_speed;
            }
        }
    }

    void EnemyComponent::setCooldown(float cooldownSeconds) {
        m_cooldown = sf::seconds(cooldownSeconds);
    }

    float EnemyComponent::getCooldown() {
        return m_cooldown.asSeconds();
    }

    EnemyComponent::EnemyComponent(float speed) {
        m_speed = speed;
    }
}