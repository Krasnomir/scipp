#include <Scriptable/Components/ProjectileComponent.hpp>
#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Scriptable/Components/HealthComponent.hpp>
#include <Scriptable/Entity.hpp>
#include <Game.hpp>

#include <iostream>

namespace Scriptable::Components {
    ProjectileComponent::ProjectileComponent(float damage, float speed, float rotation, std::string groupName) {

        m_damage = damage;
        m_targetGroup = groupName;
        m_speed = speed;
    }

    void ProjectileComponent::beforeRender(const EventData* data) {

        // update the physics component
        Entity* entity = (Entity*) this->parentEntity; // cast parentEntity generic pointer to an Entity pointer
        auto* entity_pc = entity->getComponent<Scriptable::Components::PhysicsComponent>();
        auto* entity_rc = entity->getComponent<Scriptable::Components::RenderComponent>();

        entity_pc->velocity.magnitude = m_speed;
        entity_pc->velocity.direction = m_rotation;

        // checks if the bullet is colliding with the closest target from a targetGroup
        // if it is, deal damage to the target and delete the bullet
        auto* state = Scipp::globalGame->stateManager.currentState;

        auto* target = state->findClosestEntityFromGroup(entity, m_targetGroup);
        
        if(target == nullptr) return; // no target found

        auto* target_rc = target->getComponent<Scriptable::Components::RenderComponent>();
        auto* target_hc = target->getComponent<Scriptable::Components::HealthComponent>();

        if(target_rc == nullptr || target_hc == nullptr) return;

        if(target_rc->isColliding(entity_rc)) {
            target_hc->setHealth(target_hc->getHealth() - m_damage);

            std::cout << target_hc->getHealth() << "\n";

			state->softDeleteEntity(entity->getName());
        }
        
    }
}