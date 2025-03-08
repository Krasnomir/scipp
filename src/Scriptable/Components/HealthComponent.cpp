#include <Scriptable/Components/HealthComponent.hpp>

#include <iostream>

namespace Scriptable::Components {
    void HealthComponent::beforeRender(const EventData* data) {
        if(m_health <= 0) {
            m_onDeathCallback(this);
        }
        if(m_canRegen) {
            // if enough time has passed without taking damage, start regenerating health
            if(m_timeWithoutDamage >= m_regenDelay) {
                if(m_health < m_maxHealth) setHealth(m_health + m_regen);
            }
            // increase time without taking damage by the time that has passed since last frame
            else {
                m_timeWithoutDamage += data->deltaTime;
            }
        }

        //std::cout << "Time without damage:" << m_timeWithoutDamage.asSeconds() << "\n";
        //std::cout << "Health:" << m_health << "\n";
    }

    float HealthComponent::getHealth() {
        return m_health;
    }

    void HealthComponent::setHealth(float health) {
        float currentHealth = m_health;
        if(health <= currentHealth) m_timeWithoutDamage = sf::microseconds(0); // reset the time without taking damage if we took damage
        m_health = health;
    }

    void HealthComponent::setOnDeathCallback(std::function<void(HealthComponent*)> onDeath) {
        m_onDeathCallback = onDeath;
    }
}