#include <Scriptable/Component.hpp>
#include <Scriptable/Components/SFMLRenderComponent.hpp>
#include <vector>
#include <SFML/Graphics.hpp>


namespace Scriptable::Components
{
    SFMLRenderComponent::SFMLRenderComponent(const std::vector<sf::Vector2f>& points) {
        m_vertices = sf::VertexArray(sf::TriangleStrip, points.size());
        m_verticesCount = (int) points.size();

        for(size_t i = 0; i < points.size(); i++){
            m_vertices[i] = points[i];
        }
    }

    sf::Vector2f SFMLRenderComponent::center() {
        float xSum = 0;
        float ySum = 0;

        for (int i = 0; i < m_verticesCount; i++) {
            xSum += m_vertices[i].position.x;
            ySum += m_vertices[i].position.y;
        }

        return sf::Vector2f(xSum / m_verticesCount, ySum / m_verticesCount);
    }

    void SFMLRenderComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        states.texture = &m_texture;

        target.draw(m_vertices, states);
    }

    void SFMLRenderComponent::onRender(const EventData* data){
        data->targetWindow->draw(*this);
    }
}

