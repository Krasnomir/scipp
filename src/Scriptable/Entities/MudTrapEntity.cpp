#include <Scriptable/Entities/MudTrapEntity.hpp>

#include <Scriptable/Components/RenderComponent.hpp>
#include <Scriptable/Components/HealthComponent.hpp>
#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Scriptable/Components/EnemyComponent.hpp>

namespace Scriptable::Entities {

    const std::string MudTrapEntity::M_TARGET_GROUP_NAME = "hostile";

    const std::vector<sf::Vector2f> MudTrapEntity::M_VERTICES = {
        {0,0},{100,0},{0,100},
        {100,0},{100,100},{0,100}
    };

    const int MudTrapEntity::M_DURABILITY = 50;
    const int MudTrapEntity::M_SLOWDOWN_RATE = 5;

    MudTrapEntity::MudTrapEntity(sf::Vector2f pos) {
        addComponent<Scriptable::Components::RenderComponent>(M_VERTICES);
        addComponent<Scriptable::Components::HealthComponent>(M_DURABILITY);

        auto* rc = getComponent<Scriptable::Components::RenderComponent>();
		rc->setOrigin(rc->center());
		rc->setPosition(pos);
        rc->setColor(sf::Color(86, 72, 43));

        auto* hc = getComponent<Scriptable::Components::HealthComponent>();
        hc->setOnDeathCallback(deleteTrapCallback);

        zindex = 1;
    }

    void MudTrapEntity::beforeRender(const EventData* data) {
        auto* trap_rc = getComponent<Scriptable::Components::RenderComponent>();
        auto* trap_hc = getComponent<Scriptable::Components::HealthComponent>();

        auto enemies = data->currentState->getEntitiesFromGroup(M_TARGET_GROUP_NAME);

        for(auto enemy : enemies) {
            auto* enemy_rc = enemy->getComponent<Scriptable::Components::RenderComponent>();
            auto* enemy_ec = enemy->getComponent<Scriptable::Components::EnemyComponent>();
            auto* enemy_pc = enemy->getComponent<Scriptable::Components::PhysicsComponent>();

            if(enemy_rc->isColliding(trap_rc)) {
                if(m_affectedEntities.find(enemy->getName()) == m_affectedEntities.end()) {
                    m_affectedEntities.insert(enemy->getName());
                    trap_hc->setHealth(trap_hc->getHealth() - 1);
                }

                enemy_pc->velocity = Util::vec_normalize(enemy_pc->velocity) * (enemy_ec->get_max_speed() / M_SLOWDOWN_RATE);
            }
        }
    }

}