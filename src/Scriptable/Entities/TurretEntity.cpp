#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Scriptable/Entities/TurretEntity.hpp>
#include <Scriptable/Entities/BulletEntity.hpp>
#include <GameState.hpp>

namespace Scriptable::Entities {
    TurretEntity::TurretEntity(sf::Vector2f pos) {

        addComponent<Scriptable::Components::RenderComponent>(m_vertices);
		//addComponent<Scriptable::Components::TurretComponent>(damage, firerate, group, targetGroup);
		addComponent<Scriptable::Components::HealthComponent>(m_health);

        auto* rc = getComponent<Scriptable::Components::RenderComponent>();
		rc->setOrigin(rc->center());
		rc->setPosition(pos);
        rc->setColor(sf::Color(116, 188, 218));

        addComponent<Scriptable::Components::PhysicsComponent>(rc);
        auto* pc = getComponent<Scriptable::Components::PhysicsComponent>();
        pc->collidable = true;

		getComponent<Scriptable::Components::HealthComponent>()->setOnDeathCallback(deleteTurretCallback);

		Scipp::globalGame->stateManager.currentState->addEntityToGroup(this, m_group);
    }

    void TurretEntity::beforeRender(const EventData* data) {
        m_cooldownTracker += data->deltaTime;

        auto* target = Scipp::globalGame->stateManager.currentState->findClosestEntityFromGroup(this, m_targetGroup);

        if(target==nullptr) return;

        auto* turretRC = getComponent<Scriptable::Components::RenderComponent>();
        auto* targetRC = target->getComponent<Scriptable::Components::RenderComponent>();

        if(Util::getDistanceBetweenPoints(turretRC->getPosition(), targetRC->getPosition()) < m_visionRange) {
            turretRC->setRotation(Util::getAngleBetweenPoints(turretRC->getPosition(), targetRC->getPosition()));

            if(m_cooldownTracker >= m_firerate) {
                m_cooldownTracker = sf::microseconds(0);

                static uint32_t turretProj_ID = 0;

                auto* rc = getComponent<Scriptable::Components::RenderComponent>();
                sf::Vector2f bulletStartPosition = Util::movePoint(rc->getPosition(), m_bulletOffset, rc->getRotation());

                Scipp::globalGame->stateManager.currentState->addEntity<BulletEntity>(std::to_string(turretProj_ID), rc->getRotation(), bulletStartPosition, m_damage);

                turretProj_ID++;
            }
        }
    }
}