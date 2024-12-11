#include <Scriptable/Component.hpp>
#include <Scriptable/Components/RenderComponent.hpp>

#include <SFML/Graphics.hpp>

#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>

namespace Scriptable::Components
{
    RenderComponent::RenderComponent(const std::vector<sf::Vector2f>& points) {
        if((points.size() % 3) != 0){
            throw std::runtime_error("Invalid initalization of RenderComponent");
        }

        m_vertices = sf::VertexArray(sf::Triangles, points.size());
        m_verticesCount = (int) points.size();

        for(size_t i = 0; i < points.size(); i++){
            m_vertices[i].position = points[i];
            m_vertices[i].texCoords = points[i];
        }

        boundingBoxInit();
    }

    RenderComponent::RenderComponent(const std::vector<std::pair<sf::Vector2f, sf::Vector2f>>& points){
        if((points.size() % 3) != 0){
            throw std::runtime_error("Invalid initalization of RenderComponent");
        }

        m_vertices = sf::VertexArray(sf::Triangles, points.size());
        m_verticesCount = (int) points.size();

        for(size_t i = 0; i < points.size(); i++){
            m_vertices[i].position = points[i].first;
            m_vertices[i].texCoords = points[i].second;
        }

        boundingBoxInit();
    }

    sf::Vector2f RenderComponent::center() {
        float xSum = 0;
        float ySum = 0;

        for (int i = 0; i < m_verticesCount; i++) {
            xSum += m_vertices[i].position.x;
            ySum += m_vertices[i].position.y;
        }

        return sf::Vector2f(xSum / m_verticesCount, ySum / m_verticesCount);
    }

    void RenderComponent::draw(sf::RenderTarget& target, sf::RenderStates states) const {
        states.transform *= getTransform();
        
        states.texture = &m_texture;

        target.draw(m_vertices, states);
    }

    void RenderComponent::onRender(const EventData* data){
        data->targetWindow->draw(*this);
    }

    void RenderComponent::addCostume(std::string name, std::string path, sf::IntRect area) {
        std::unique_lock<std::shared_mutex> writeLock(m_costumesLock);

        sf::Texture tex;
        tex.loadFromFile(path, area);

        m_costumes[name] = tex;
    }

    void RenderComponent::addCostume(std::string name, std::string path) {
        std::unique_lock<std::shared_mutex> writeLock(m_costumesLock);

        sf::Texture tex;
        tex.loadFromFile(path);

        m_costumes[name] = tex;
    }

    void RenderComponent::loadCostume(std::string name) {
        std::shared_lock<std::shared_mutex> readLock(m_costumesLock);

        m_texture = m_costumes[name];
    }

    // this bounding box is always going to contain the vertex array shape regardless of its rotation
    // the downside to this approach is that the bounding box can be way bigger than the actual vertex array shape
    void RenderComponent::boundingBoxInit() {
        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::lowest();
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::lowest();

        for(int i = 0; i < m_verticesCount; i++) {
            minX = std::min(minX, m_vertices[i].position.x);
            maxX = std::max(maxX, m_vertices[i].position.x);
            minY = std::min(minY, m_vertices[i].position.y);
            maxY = std::max(maxY, m_vertices[i].position.y);
        }

        float size = std::max(maxX, maxY);

        m_boundingBoxSize = size / 2;
        m_boundingBox = sf::FloatRect(minX, minY, size, size);
    }

    sf::FloatRect RenderComponent::getBoundingBox() {
        return m_boundingBox;
    }

    float RenderComponent::getBoundingBoxSize() {
        return m_boundingBoxSize;
    }

    // this is just for quick collision detection using bounding boxes this is not fully accurate method of checking collissions but its ffast
    // TODO: proper collision detection that's going to go through each triangle and see if they collide with eachother
    bool RenderComponent::boundingBoxCollide(Scriptable::Components::RenderComponent* renderComponent) {
        sf::FloatRect boundingBox1 = getBoundingBox();
        sf::FloatRect boundingBox2 = renderComponent->getBoundingBox();

        sf::Vector2f position1 = getPosition();
        sf::Vector2f position2 = renderComponent->getPosition();

        boundingBox1.left += position1.x - m_boundingBoxSize;
        boundingBox1.top += position1.y - m_boundingBoxSize;

        boundingBox2.left += position2.x - renderComponent->getBoundingBoxSize();
        boundingBox2.top += position2.y - renderComponent->getBoundingBoxSize();

        //std::cout << boundingBox1.left << " " << boundingBox1.top << " | " << boundingBox1.height << " | " << boundingBox1.width << "\n";
        //std::cout << boundingBox2.left << " " << boundingBox2.top << " | " << boundingBox2.height << " | " << boundingBox2.width << "\n\n";

        return boundingBox1.intersects(boundingBox2);
    }

    sf::VertexArray RenderComponent::getVertices() {
        return m_vertices;
    }
}

