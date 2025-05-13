#ifndef __ENEMY_COMPONENT
#define __ENEMY_COMPONENT

#include <Scriptable/Component.hpp>
#include <Scriptable/Entity.hpp>
#include <Util.hpp>


namespace Scriptable::Components {

    class EnemyComponent : public Component {

        Entity* m_target;

        sf::Time m_cooldown = sf::seconds(1);
        sf::Time m_cooldownTracker = sf::microseconds(0);

        float m_speed = 0;
        float m_maxSpeed = 0;

    public:

        float damage = 20;
        float reach = 50;

        EnemyComponent(float speed, Util::shape_t vertices);
        virtual ~EnemyComponent() = default;

        void setCooldown(float cooldownSeconds);
        float getCooldown();

        float getMaxSpeed();

        void beforeRender(const EventData* data);

        float get_max_speed();
    };
}

#endif