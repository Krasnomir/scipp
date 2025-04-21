#ifndef __MUD_TRAP_ENTITY
#define __MUD_TRAP_ENTITY

#include <Scriptable/Entity.hpp>

#include <Scriptable/Components/HealthComponent.hpp>

#include <Game.hpp>

#include <set>

namespace Scriptable::Entities {

    class MudTrapEntity : public Scriptable::Entity {
        
        static const std::string M_TARGET_GROUP_NAME;
        static const std::vector<sf::Vector2f> M_VERTICES;
        static const int M_DURABILITY;
        static const int M_SLOWDOWN_RATE;
        
        // the trap loses durability when a enemy enters it for a first time
        std::set<std::string> m_affectedEntities;

        static void deleteTrapCallback(Scriptable::Components::HealthComponent* c) {
            Scipp::globalGame->stateManager.currentState->softDeleteEntity(((Scriptable::Entity*)c->parentEntity)->getName());
        }
    
    public:
        MudTrapEntity(sf::Vector2f pos);

        virtual ~MudTrapEntity() = default;

        void beforeRender(const EventData* data);
    };
}

#endif
