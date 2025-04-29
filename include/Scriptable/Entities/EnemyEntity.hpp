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
        void drop_item();

        enum Type {
            normal,
            speedy,
            tank,
            boss
        };

        typedef std::map<ItemEntity::Item, short> item_drop_info; 

    private:

        static const std::map<Type, item_drop_info> TYPE_DROP_INFO;

        static const std::map<Type, EnemyTypeInfo> TYPE_INFO;

        Type m_type;

        std::string groupName = "hostile";

        static void deleteEnemyCallback(Scriptable::Components::HealthComponent* c) {

            using namespace Scriptable::Entities;
            using namespace Scriptable::Components;

            auto* currentState = Scipp::globalGame->stateManager.currentState;
            auto* entity = (EnemyEntity*)c->parentEntity;

            currentState->softDeleteEntity(entity->getName());

            entity->drop_item();
        }

    public:

        EnemyEntity(sf::Vector2f pos, Type type, float healthMultiplier);
        virtual ~EnemyEntity() = default;
    };
}

#endif
