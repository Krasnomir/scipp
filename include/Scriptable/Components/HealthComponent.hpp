#ifndef __HEALTH_COMPONENT
#define __HEALTH_COMPONENT

#include <functional>

#include <Scriptable/Component.hpp>

namespace Scriptable::Components {
    
    class HealthComponent : public Component {
        std::function<void(HealthComponent*)> m_onDeathCallback = 0;

        sf::Time m_timeWithoutDamage;

        float m_health = 0;
        float m_maxHealth = 0;
        bool m_canRegen = false;
        float m_regen = 0;
        sf::Time m_regenDelay = sf::microseconds(0); // how much time of not taking damage it takes to start regenerating health, in seconds

    public:

        float getHealth();
        void setHealth(float health);

        void setOnDeathCallback(std::function<void(HealthComponent*)> onDeath); 

        HealthComponent() = delete;
        
        HealthComponent(float health) : m_health(health), m_maxHealth(health) {};
        HealthComponent(float health, float maxHealth) : m_health(health), m_maxHealth(maxHealth) {};
        HealthComponent(float health, float maxHealth, float regen) : m_health(health), m_maxHealth(maxHealth), m_regen(regen), m_canRegen(true) {};
        HealthComponent(float health, float maxHealth, float regen, float regenDelay) : m_health(health), m_maxHealth(maxHealth), m_regen(regen), m_regenDelay(sf::seconds(regenDelay)), m_canRegen(true) {};

        virtual ~HealthComponent() = default;

        void beforeRender(const EventData* data);
    };
}


#endif