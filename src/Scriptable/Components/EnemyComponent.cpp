#include <Scriptable/Components/EnemyComponent.hpp>
#include <Game.hpp>

#include <iostream>

namespace Scriptable::Components {
    void EnemyComponent::beforeRender(const EventData* data) {
        Entity* parentEntity = (Entity*) this->parentEntity; // cast parentEntity generic pointer to an Entity pointer
        m_target = Scipp::globalGame->stateManager.currentState->findClosestEntityFromGroup(parentEntity, "friendly");

        if(m_target != nullptr) {

            auto* entity_rc = parentEntity->getComponent<Scriptable::Components::RenderComponent>();
            auto* target_rc = m_target->getComponent<Scriptable::Components::RenderComponent>();
            entity_rc->setRotation(Util::getAngleBetweenPoints(entity_rc->getPosition(), target_rc->getPosition()));
        }
    }

    EnemyComponent::EnemyComponent() {}
}