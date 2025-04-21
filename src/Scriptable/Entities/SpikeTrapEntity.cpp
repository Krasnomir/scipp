#include <Scriptable/Entities/SpikeTrapEntity.hpp>

#include <Scriptable/Components/RenderComponent.hpp>
#include <Scriptable/Components/HealthComponent.hpp>
#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Scriptable/Components/EnemyComponent.hpp>

namespace Scriptable::Entities {

    const std::string SpikeTrapEntity::M_TARGET_GROUP_NAME = "hostile";

    const std::vector<sf::Vector2f> SpikeTrapEntity::M_VERTICES = {
        {0,0},{100,0},{0,100},
        {100,0},{100,100},{0,100}
    };

    const int SpikeTrapEntity::M_DURABILITY = 20;
    const int SpikeTrapEntity::M_DAMAGE = 20;
    const sf::Time SpikeTrapEntity::M_DAMAGE_COOLDOWN = sf::milliseconds(500);

    SpikeTrapEntity::SpikeTrapEntity(sf::Vector2f pos) {
        addComponent<Scriptable::Components::RenderComponent>(M_VERTICES);
        addComponent<Scriptable::Components::HealthComponent>(200);

        auto* rc = getComponent<Scriptable::Components::RenderComponent>();
		rc->setOrigin(rc->center());
		rc->setPosition(pos);
        rc->setColor(sf::Color(165, 165, 165));

        auto* hc = getComponent<Scriptable::Components::HealthComponent>();
        hc->setOnDeathCallback(deleteTrapCallback);
    }

    void SpikeTrapEntity::beforeRender(const EventData* data) {
        auto* trap_rc = getComponent<Scriptable::Components::RenderComponent>();
        auto* trap_hc = getComponent<Scriptable::Components::HealthComponent>();

        auto enemies = data->currentState->getEntitiesFromGroup(M_TARGET_GROUP_NAME);

        for(auto enemy : enemies) {
            auto* enemy_rc = enemy->getComponent<Scriptable::Components::RenderComponent>();

            if(enemy_rc->isColliding(trap_rc)) {
                if(m_affectedEntities.find(enemy->getName()) == m_affectedEntities.end()) {
                    m_affectedEntities.insert(enemy->getName());
                    trap_hc->setHealth(trap_hc->getHealth() - 1);
                }

                m_enemyDamageMapLock.lock();
                if(m_enemyDamageMap.find(enemy->getName()) == m_enemyDamageMap.end()) {
                    m_enemyDamageMap[enemy->getName()] = sf::seconds(0);
                }

                if(m_enemyDamageMap[enemy->getName()] <= sf::seconds(0)) {
                    auto* enemy_hc = enemy->getComponent<Scriptable::Components::HealthComponent>();

                    enemy_hc->setHealth(enemy_hc->getHealth() - M_DAMAGE);
                    m_enemyDamageMap[enemy->getName()] = M_DAMAGE_COOLDOWN;
                }
                else {
                    m_enemyDamageMap[enemy->getName()] -= data->deltaTime;
                }
                m_enemyDamageMapLock.unlock();
            }
        }
    }

}