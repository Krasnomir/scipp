#ifndef __PHYSICS_COMPONENT
#define __PHYSICS_COMPONENT 1

#include <Scriptable/Component.hpp>
#include <Scriptable/Components/RenderComponent.hpp>

namespace Scriptable::Components {

    class PhysicsComponent : public Component {

    public:
        float speed;
        float velocity;
        Scriptable::Components::RenderComponent* renderComponent;

        PhysicsComponent(float b, Scriptable::Components::RenderComponent* rc);
        virtual ~PhysicsComponent() = default;

        void beforeRender(const EventData* data);
    };
}

#endif
