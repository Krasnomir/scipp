#ifndef ENEMY_ENTITY
#define ENEMY_ENTITY

#include <Scriptable/Entity.hpp>

#include <Scriptable/Entities/ItemEntity.hpp>

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

            using namespace Scriptable::Entities;
            using namespace Scriptable::Components;

            static uint32_t item_id = 0;

            auto* entity = (Entity*)c->parentEntity;
            auto* rc = entity->getComponent<RenderComponent>();

            auto* currentState = Scipp::globalGame->stateManager.currentState;

            currentState->addEntity<ItemEntity>("item_" + item_id, ItemEntity::Item::steel, rc->getPosition());
            currentState->softDeleteEntity(entity->getName());

            ++item_id;
        }

public:

        EnemyEntity(sf::Vector2f pos);
        virtual ~EnemyEntity() = default;
    };
}

#endif
