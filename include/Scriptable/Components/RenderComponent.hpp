#ifndef __RENDER_COMPONENT
#define __RENDER_COMPONENT 1

#include <Scriptable/Component.hpp>
#include <SFML/Graphics.hpp>
#include <Util.hpp>

#include <string>
#include <vector>
#include <unordered_map> 

namespace Scriptable::Components{
    class RenderComponent : public Component, public sf::Drawable, public sf::Transformable {
    public:
        RenderComponent() = delete;

        //WARNING: This function requires input in the format of SFML triangles
        RenderComponent(const Util::shape_t& trianglePoints);

        //WARNING: This function requires input in the format of pairs of {triangle_vertex : txt_coord};
        RenderComponent(const std::vector<std::pair<sf::Vector2f, sf::Vector2f>>& trianglePointsTxtCoords);

        virtual ~RenderComponent() = default;

        sf::Vector2f center();

        void onRender(const EventData* data);

        void addCostume(std::string name, std::string path, sf::IntRect area);
        void addCostume(std::string name, std::string path);
        void loadCostume(std::string name);
    private:
        int m_verticesCount;

        sf::VertexArray m_vertices;
        sf::Texture m_texture;

        mutable std::shared_mutex m_costumesLock;
        std::unordered_map<std::string, sf::Texture> m_costumes;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    };
}

#endif