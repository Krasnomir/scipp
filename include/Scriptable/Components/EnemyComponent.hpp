#ifndef __ENEMY_COMPONENT
#define __ENEMY_COMPONENT

#include <Scriptable/Component.hpp>
#include <Scriptable/Entity.hpp>

namespace Scriptable::Components {

    class EnemyComponent : public Component {

        Entity* m_target;

        sf::Time m_cooldown = sf::seconds(1);
        sf::Time m_cooldownTracker = sf::microseconds(0);

    public:

        float damage = 20;
        float reach = 50;

        EnemyComponent();
        virtual ~EnemyComponent() = default;

        void setCooldown(float cooldownSeconds);
        float getCooldown();

        void beforeRender(const EventData* data);
    };
}

#endif