#ifndef ENEMY_ENTITY
#define ENEMY_ENTITY

#include <Scriptable/Entity.hpp>

#include <Scriptable/Entities/ItemEntity.hpp>

#include <Scriptable/Components/HealthComponent.hpp>

#include <Game.hpp>

#include <iostream>

namespace Scriptable::Entities {
    struct EnemyTypeInfo {
        std::vector<sf::Vector2f> vertices;
        int health;
        float speed;
        int damage;
        sf::Color color;
    };

    class EnemyEntity : public Scriptable::Entity {

public:
        enum Type {
            normal,
            speedy,
            tank,
            boss
        };

private:

        static const std::map<Type, EnemyTypeInfo> TYPE_INFO;

        std::string groupName = "hostile";

        static void deleteEnemyCallback(Scriptable::Components::HealthComponent* c) {

            using namespace Scriptable::Entities;
            using namespace Scriptable::Components;

            auto* currentState = Scipp::globalGame->stateManager.currentState;
            auto* entity = (Entity*)c->parentEntity;

            short random = rand() % 3 + 1;

            if(random == 1) {
                static uint32_t item_id = 0;

                auto* rc = entity->getComponent<RenderComponent>();
                currentState->addEntity<ItemEntity>("item_" + item_id, ItemEntity::Item::steel, rc->getPosition());

                ++item_id;
            }

            currentState->softDeleteEntity(entity->getName());
        }

    public:

        EnemyEntity(sf::Vector2f pos, Type type);
        virtual ~EnemyEntity() = default;
    };
}

#endif
