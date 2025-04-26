#ifndef __PHYSICS_COMPONENT
#define __PHYSICS_COMPONENT 1

#include <Scriptable/Component.hpp>
#include <Scriptable/Components/RenderComponent.hpp>

namespace Scriptable::Components {

    class PhysicsComponent : public Component {
        RenderComponent* m_renderComponent;

    public:

        bool collidable = false;

        sf::Vector2f velocity;

        PhysicsComponent(RenderComponent* renderComponent);
        virtual ~PhysicsComponent() = default;

        void beforeRender(const EventData* data);
    };
}

#endif
