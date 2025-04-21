#ifndef __SPIKE_TRAP_ENTITY
#define __SPIKE_TRAP_ENTITY

#include <Scriptable/Entity.hpp>

#include <Scriptable/Components/HealthComponent.hpp>

#include <Game.hpp>

#include <mutex>

namespace Scriptable::Entities {

    class SpikeTrapEntity : public Scriptable::Entity {
        
        static const std::string M_TARGET_GROUP_NAME;
        static const std::vector<sf::Vector2f> M_VERTICES;
        static const int M_DURABILITY;
        static const int M_DAMAGE;
        static const sf::Time M_DAMAGE_COOLDOWN;

        std::mutex m_enemyDamageMapLock;
        std::map<std::string, sf::Time> m_enemyDamageMap;

        // the trap loses durability when a enemy enters it for a first time
        std::set<std::string> m_affectedEntities;
    
        static void deleteTrapCallback(Scriptable::Components::HealthComponent* c) {
            Scipp::globalGame->stateManager.currentState->softDeleteEntity(((Scriptable::Entity*)c->parentEntity)->getName());
        }
    
    public:
        SpikeTrapEntity(sf::Vector2f pos);

        virtual ~SpikeTrapEntity() = default;

        void beforeRender(const EventData* data);
    };
}

#endif
