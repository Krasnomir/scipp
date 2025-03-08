#ifndef __PHYSICS_COMPONENT
#define __PHYSICS_COMPONENT 1

#include <Scriptable/Component.hpp>
#include <Scriptable/Components/RenderComponent.hpp>

namespace Scriptable::Components {

    class PhysicsComponent : public Component {
        RenderComponent* m_renderComponent;

    public:
        struct Velocity {
            float magnitude = 0;
            float direction = 0;
        };
        Velocity velocity;

        PhysicsComponent(RenderComponent* renderComponent);
        virtual ~PhysicsComponent() = default;

        void beforeRender(const EventData* data);
    };
}

#endif
