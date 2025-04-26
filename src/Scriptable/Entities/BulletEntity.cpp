#include <Scriptable/Entities/BulletEntity.hpp>

#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Scriptable/Components/HealthComponent.hpp>

#include <Misc/Explosion.hpp>

#include <iostream>
#include <cmath>

namespace Scriptable::Entities {
    BulletEntity::BulletEntity(float angle, sf::Vector2f pos, int damage) {
        m_damage = damage;

        addComponent<Scriptable::Components::RenderComponent>(m_vertices);
		addComponent<Scriptable::Components::LifetimeComponent>(sf::seconds(m_lifetimeSeconds), deleteBulletCallback);
		addComponent<Scriptable::Components::PhysicsComponent>(getComponent<Scriptable::Components::RenderComponent>());
		//addComponent<Scriptable::Components::ProjectileComponent>(damage, speed, angle, targetGroupName);

        auto* rc = getComponent<Scriptable::Components::RenderComponent>();
		rc->setOrigin(getComponent<Scriptable::Components::RenderComponent>()->center());
		rc->setPosition(pos);
		rc->setRotation(angle);
        rc->setColor(sf::Color(255, 189, 58));

        auto* pc = getComponent<Scriptable::Components::PhysicsComponent>();
        pc->velocity = Util::vec_from_mag_and_dir(m_speed, angle);

		Scipp::globalGame->stateManager.currentState->addEntityToGroup(this, m_group);

        zindex = 3;
    }

    void BulletEntity::beforeRender(const EventData* data) {

        // update the physics component
        auto* bullet_pc = getComponent<Scriptable::Components::PhysicsComponent>();
        auto* bullet_rc = getComponent<Scriptable::Components::RenderComponent>();

        // checks if the bullet is colliding with the closest target from a targetGroup
        // if it is, deal damage to the target and delete the bullet
        auto* state = Scipp::globalGame->stateManager.currentState;

        auto* target = state->findClosestEntityFromGroup(this, m_targetGroup);
        
        if(target == nullptr) return; // no target found

        auto* target_rc = target->getComponent<Scriptable::Components::RenderComponent>();
        auto* target_hc = target->getComponent<Scriptable::Components::HealthComponent>();

        if(target_rc == nullptr || target_hc == nullptr) return;

        if(target_rc->isColliding(bullet_rc)) {

            int particleCount = 5;
            int size = 20;
            int speed = 4;
            float duration = 0.7; // in seconds
            explosion(bullet_rc->getPosition(), particleCount, size, speed, duration, sf::Color(150, 60, 60));

            target_hc->setHealth(target_hc->getHealth() - m_damage);
			state->softDeleteEntity(getName());
        }
        
    }
}