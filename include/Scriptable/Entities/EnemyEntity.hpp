#ifndef ENEMY_ENTITY
#define ENEMY_ENTITY

#include <Scriptable/Entity.hpp>

#include <Scriptable/Components/HealthComponent.hpp>

#include <Game.hpp>

namespace Scriptable::Entities {
    class EnemyEntity : public Scriptable::Entity {
        float health = 100;
        std::string groupName = "hostile";

        std::vector<sf::Vector2f> vertices = {
            {25,25}, {25,50}, {10,40}, 
            {25,25}, {0,25}, {10,40},
            {25,25}, {0,25}, {10,10},
            {25,25}, {25,0}, {10,10},
            {25,25}, {25,0}, {40,10},
            {25,25}, {50,25}, {40,10},
            {25,25}, {50,25}, {40,40},
            {25,25}, {25,50}, {40,40},
        };

        static void deleteEnemyCallback(Scriptable::Components::HealthComponent* c) {
            Scipp::globalGame->stateManager.currentState->softDeleteEntity(((Scriptable::Entity*)c->parentEntity)->getName());
        }

public:

        EnemyEntity(sf::Vector2f pos);
        virtual ~EnemyEntity() = default;
    };
}

#endif
