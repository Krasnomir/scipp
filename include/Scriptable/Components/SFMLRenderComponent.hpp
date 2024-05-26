#ifndef __RENDER_COMPONENT
#define __RENDER_COMPONENT 1

#include <Scriptable/Component.hpp>
#include <vector>


namespace Scriptable::Components{

    class SFMLRenderComponent : public Component, public sf::ConvexShape{
    public:
        SFMLRenderComponent() = default;
        SFMLRenderComponent(const std::vector<sf::Vector2f>& points);

        void onRender(const EventData* data);

        ~SFMLRenderComponent() = default;
    private:

    };
}


#endif