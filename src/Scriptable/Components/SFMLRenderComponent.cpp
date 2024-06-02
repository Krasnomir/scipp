#include <Scriptable/Component.hpp>
#include <Scriptable/Components/SFMLRenderComponent.hpp>

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

namespace Scriptable::Components
{
    SFMLRenderComponent::SFMLRenderComponent(const std::vector<sf::Vector2f>& points) {
        m_vertices = sf::VertexArray(sf::TriangleStrip, points.size());
        m_verticesCount = (int) points.size();

        for(size_t i = 0; i < points.size(); i++){
            m_vertices[i].position = points[i];
            m_vertices[i].texCoords = points[i];
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

    void SFMLRenderComponent::addCostume(std::string name, std::string path, sf::IntRect area) {
        std::unique_lock<std::shared_mutex> writeLock(m_costumesLock);

        sf::Texture tex;
        tex.loadFromFile(path, area);

        m_costumes[name] = tex;
    }

    void SFMLRenderComponent::addCostume(std::string name, std::string path) {
        std::unique_lock<std::shared_mutex> writeLock(m_costumesLock);

        sf::Texture tex;
        tex.loadFromFile(path);

        m_costumes[name] = tex;
    }

    void SFMLRenderComponent::loadCostume(std::string name) {
        std::shared_lock<std::shared_mutex> readLock(m_costumesLock);

        m_texture = m_costumes[name];
    }
}

