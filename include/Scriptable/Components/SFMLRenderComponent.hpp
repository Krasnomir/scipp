#ifndef __RENDER_COMPONENT
#define __RENDER_COMPONENT 1

#include <Scriptable/Component.hpp>
#include <SFML/Graphics.hpp>

#include <vector>
#include <unordered_map> 

namespace Scriptable::Components{

    class SFMLRenderComponent : public Component, public sf::Drawable, public sf::Transformable {
    public:
        //SFMLRenderComponent() = default;

        //WARNING: This function requires input in the format of SFML triangleStrip
        SFMLRenderComponent(const std::vector<sf::Vector2f>& triangleStripPoints);
        ~SFMLRenderComponent() = default;

        sf::Vector2f center();

        void onRender(const EventData* data);

        void addCostume(int id, sf::Texture texture);
        void loadCostume(int id);
    private:
        int m_verticesCount;

        sf::VertexArray m_vertices;
        sf::Texture m_texture;

        std::unordered_map<int, sf::Texture> m_costumes;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };
}


#endif