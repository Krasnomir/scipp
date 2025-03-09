#ifndef __ENEMY_COMPONENT
#define __ENEMY_COMPONENT

#include <Scriptable/Component.hpp>
#include <Scriptable/Entity.hpp>

namespace Scriptable::Components {

    class EnemyComponent : public Component {

        Entity* m_target;

    public:

        EnemyComponent();
        virtual ~EnemyComponent() = default;

        void beforeRender(const EventData* data);
    };
}

#endif