#include <Scriptable/Entities/BulletEntity.hpp>

#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Scriptable/Components/HealthComponent.hpp>

#include <Misc/Explosion.hpp>

namespace Scriptable::Entities {
    BulletEntity::BulletEntity(float angle, sf::Vector2f pos) {
        addComponent<Scriptable::Components::RenderComponent>(m_vertices);
		addComponent<Scriptable::Components::LifetimeComponent>(sf::seconds(m_lifetimeSeconds), deleteBulletCallback);
		addComponent<Scriptable::Components::PhysicsComponent>(getComponent<Scriptable::Components::RenderComponent>());
		//addComponent<Scriptable::Components::ProjectileComponent>(damage, speed, angle, targetGroupName);

		getComponent<Scriptable::Components::RenderComponent>()->setOrigin(getComponent<Scriptable::Components::RenderComponent>()->center());
		getComponent<Scriptable::Components::RenderComponent>()->setPosition(pos);
		getComponent<Scriptable::Components::RenderComponent>()->setRotation(angle);

		Scipp::globalGame->stateManager.currentState->addEntityToGroup(this, m_group);
    }

    void BulletEntity::beforeRender(const EventData* data) {

        // update the physics component
        auto* bullet_pc = getComponent<Scriptable::Components::PhysicsComponent>();
        auto* bullet_rc = getComponent<Scriptable::Components::RenderComponent>();

        bullet_pc->velocity.magnitude = m_speed;
        bullet_pc->velocity.direction = bullet_rc->getRotation();

        // checks if the bullet is colliding with the closest target from a targetGroup
        // if it is, deal damage to the target and delete the bullet
        auto* state = Scipp::globalGame->stateManager.currentState;

        auto* target = state->findClosestEntityFromGroup(this, m_targetGroup);
        
        if(target == nullptr) return; // no target found

        auto* target_rc = target->getComponent<Scriptable::Components::RenderComponent>();
        auto* target_hc = target->getComponent<Scriptable::Components::HealthComponent>();

        if(target_rc == nullptr || target_hc == nullptr) return;

        if(target_rc->isColliding(bullet_rc)) {
            using namespace Scipp::ExplosionPresets;

            int particleCount = 5;
            int size = 30;
            int speed = 2;
            float duration = 1; // in seconds
            explosion(bullet_rc->getPosition(), BulletHit::count, BulletHit::size, BulletHit::speed, BulletHit::duration);

            target_hc->setHealth(target_hc->getHealth() - m_damage);
			state->softDeleteEntity(getName());
        }
        
    }
}