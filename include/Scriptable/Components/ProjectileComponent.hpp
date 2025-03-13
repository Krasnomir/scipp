#ifndef __BULLET_COMPONENT
#define __BULLET_COMPONENT

#include <Scriptable/Component.hpp>

#include <string>

namespace Scriptable::Components {
    class ProjectileComponent : public Component {
        sf::Time m_lifetime;
        float m_damage;
        float m_speed;
        float m_rotation;

        std::string m_targetGroup;

    public:
        ProjectileComponent(float damage, float speed, float rotation, std::string targetGroup);
        virtual ~ProjectileComponent() = default;

        void init();

        void beforeRender(const EventData* data);
    };
}

#endif