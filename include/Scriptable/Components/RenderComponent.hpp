#ifndef __RENDER_COMPONENT
#define __RENDER_COMPONENT 1

#include <Scriptable/Component.hpp>
#include <vector>

#include <SFML/Graphics.hpp>

namespace Scriptable::Components{
    class RenderComponent : public Component, public sf::Drawable, public sf::Transformable {
    public:
        //RenderComponent() = default;

        //WARNING: This function requires input in the format of SFML triangleStrip
        RenderComponent(const std::vector<sf::Vector2f>& triangleStripPoints);
        ~RenderComponent() = default;

        sf::Vector2f center();

        void onRender(const EventData* data);

    private:
        int m_verticesCount;

        sf::VertexArray m_vertices;
        sf::Texture m_texture;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };
}


#endif