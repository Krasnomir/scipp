#ifndef __BULLET_ENTITY
#define __BULLET_ENTITY

#include <Scriptable/Entity.hpp>

#include <Scriptable/Components/LifetimeComponent.hpp>

#include <Game.hpp>

namespace Scriptable::Entities {

    class BulletEntity : public Scriptable::Entity
    {
        float m_lifetimeSeconds = 3;
        float m_damage = 20;
        float m_speed = 10;
        std::string m_group = "bullets";
        std::string m_targetGroup = "hostile";

        std::vector<sf::Vector2f> m_vertices = {{0,50}, {50,50}, {25,0}};

        static void deleteBulletCallback(Scriptable::Components::LifetimeComponent* c) {
            Scipp::globalGame->stateManager.currentState->softDeleteEntity(((Scriptable::Entity*)c->parentEntity)->getName());
        }

    public:
        BulletEntity(float angle, sf::Vector2f pos);
        virtual ~BulletEntity() = default;

        void beforeRender(const EventData* data);
    };
}

#endif
