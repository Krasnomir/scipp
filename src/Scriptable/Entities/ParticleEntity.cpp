#include <Scriptable/Entities/ParticleEntity.hpp>

namespace Scriptable::Entities {

    ParticleEntity::ParticleEntity(sf::Vector2f pos, float direction, float speed, float size, sf::Time lifetime) {

        std::vector<sf::Vector2f> vertices = {{0,0}, {0,size}, {size, 0}, {0, size}, {size,size}, {size,0}};

        addComponent<Scriptable::Components::RenderComponent>(vertices);
        addComponent<Scriptable::Components::PhysicsComponent>(getComponent<Scriptable::Components::RenderComponent>());
        addComponent<Scriptable::Components::LifetimeComponent>(lifetime, deleteParticleCallback);

        auto* rc = getComponent<Scriptable::Components::RenderComponent>();
        rc->setPosition(pos);
        rc->setOrigin(rc->center());
        rc->setRotation(direction);

        m_direction = direction;
        m_speed = speed;
    }

    void ParticleEntity::beforeRender(const EventData* data) {
        if(m_isFadingOut) {
            m_alpha -= data->deltaTime.asMilliseconds();
            
            if(m_alpha <= 0) {
                Scipp::globalGame->stateManager.currentState->softDeleteEntity(getName());
                return;
            }
            
            auto* rc = getComponent<Scriptable::Components::RenderComponent>();
            rc->setAlpha((int) m_alpha);
        }
        else {
            auto* pc = getComponent<Scriptable::Components::PhysicsComponent>();

            pc->velocity.direction = m_direction;
            pc->velocity.magnitude = m_speed;
        }
    }
}