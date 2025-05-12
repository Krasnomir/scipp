#ifndef __TURRET_ENTITY
#define __TURRET_ENTITY

#include <Scriptable/Entity.hpp>

#include <Scriptable/Components/RenderComponent.hpp>
#include <Scriptable/Components/HealthComponent.hpp>

#include <Game.hpp>

namespace Scriptable::Entities {

    class TurretEntity : public Scriptable::Entity {

        float m_health = 100;

        float m_damage = 200;
        sf::Time m_firerate = sf::seconds(0.8);

        sf::Time m_cooldownTracker = sf::microseconds(0);

        float m_visionRange = 500;
        
        float m_bulletOffset = 50;

        std::string m_group = "friendly";
        std::string m_targetGroup = "hostile";
    
        std::vector<sf::Vector2f> m_vertices = 
        {
            {0,0}, {0,40}, {40,0}, 
            {0,40}, {40,40}, {40,0}, 
            {0,40}, {0,0}, {-10,20},
            {40,40}, {0,40}, {20,50},
            {40,40}, {40,0}, {70,20},
            {0,0}, {40,0}, {20,-10},
        };
    
        static void deleteTurretCallback(Scriptable::Components::HealthComponent* c) {
            Scipp::globalGame->stateManager.currentState->softDeleteEntity(((Scriptable::Entity*)c->parentEntity)->getName());
        }
    
    public:
        TurretEntity(sf::Vector2f pos);

        virtual ~TurretEntity() = default;

        void beforeRender(const EventData* data);
    };
}

#endif
