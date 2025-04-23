#ifndef __EXPLOSION_ENTITY
#define __EXPLOSION_ENTITY

#include <Scriptable/Entity.hpp>

#include <Scriptable/Components/RenderComponent.hpp>
#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Scriptable/Components/LifetimeComponent.hpp>

#include <Game.hpp>

#include <iostream>

namespace Scriptable::Entities {

    class ParticleEntity : public Scriptable::Entity {
        
        float m_direction;
        float m_speed;
        sf::Vector2f m_pos;

        float m_alpha = 0;

        bool m_isFadingOut = false;

        static void deleteParticleCallback(Scriptable::Components::LifetimeComponent* c) {
            ((Scriptable::Entities::ParticleEntity*)c->parentEntity)->fadeOut();
            //Scipp::globalGame->stateManager.currentState->softDeleteEntity(((Scriptable::Entity*)c->parentEntity)->getName());
        }
    
    public:
        ParticleEntity(sf::Vector2f pos, float  direction, float speed, float size, sf::Time lifetime, sf::Color color);
        virtual ~ParticleEntity() = default;

        void fadeOut() {
            m_isFadingOut = true;
        }

        void beforeRender(const EventData* data);
    };
}

#endif