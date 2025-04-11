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

    void RenderComponent::setVertices(const std::vector<sf::Vector2f>& points){
        if((points.size() % 3) != 0){
            throw std::runtime_error("Invalid initalization of RenderComponent");
        }

        m_vertices = sf::VertexArray(sf::Triangles, points.size());
        m_verticesCount = (int) points.size();

        for(size_t i = 0; i < points.size(); i++){
            m_vertices[i].position = points[i];
            m_vertices[i].texCoords = points[i];
            m_vertices[i].color = m_color;
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

    // those two override the sf::Transofrmable setRotation() method, they update AABB bounding box each time they're called
    // couldn't do it with templates, maybe Yesn't will figure it out 
    void RenderComponent::setRotation(float angle) {
        sf::Transformable::setRotation(angle);

        float cosA = std::cos(angle * 3.14f / 180.0f);
        float sinA = std::sin(angle * 3.14f / 180.0f);
        m_rotationMatrix = RotationMatrix2x2(cosA, sinA);
        AABB();
    }
    void RenderComponent::setRotation(double angle) {
        sf::Transformable::setRotation(angle);

        float cosA = std::cos(angle * 3.14f / 180.0f);
        float sinA = std::sin(angle * 3.14f / 180.0f);
        m_rotationMatrix = RotationMatrix2x2(cosA, sinA);
        AABB();
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

    void RenderComponent::setAlpha(float alpha) { // values from 0 to 255
        m_alpha = alpha;

        for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i) {
            m_vertices[i].color.a = alpha;
        }
    }

    int RenderComponent::getAlpha() {
        return m_alpha;
    }

    void RenderComponent::setColor(sf::Color color) {
        m_color = color;

        for (std::size_t i = 0; i < m_vertices.getVertexCount(); ++i) {
            m_vertices[i].color = color;
        }
    }

    void RenderComponent::setColor(sf::Color color, int index) {
        m_vertices[index].color = color;
    }
    sf::Color RenderComponent::getColor() {
        return m_color;
    }

    // this bounding box is always going to contain the vertex array shape regardless of its rotation
    // the downside to this approach is that the bounding box can be way bigger than the actual vertex array shape
    /*
    void RenderComponent::rotationIndependentBoundingBox() {
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

        m_boundingBox = sf::FloatRect(minX, minY, size, size);
    }
    */

    // minimal rectangle that can contain the vertex array, has to be updated each time the rotation is changed
    void RenderComponent::AABB() {
        float minX = std::numeric_limits<float>::max();
        float maxX = std::numeric_limits<float>::lowest();
        float minY = std::numeric_limits<float>::max();
        float maxY = std::numeric_limits<float>::lowest();

        for (int i = 0; i < m_verticesCount; i++) {
            sf::Vector2f rotatedVertex = m_rotationMatrix.rotate(m_vertices[i].position);

            minX = std::min(minX, rotatedVertex.x);
            maxX = std::max(maxX, rotatedVertex.x);
            minY = std::min(minY, rotatedVertex.y);
            maxY = std::max(maxY, rotatedVertex.y);
        }

        m_boundingBox = sf::FloatRect(minX, minY, maxX - minX, maxY - minY);
    }

    
    void RenderComponent::boundingBoxInit() {
        AABB();        
    }

    sf::FloatRect RenderComponent::getBoundingBox() {
        return m_boundingBox;
    }

    bool isPointInside_fast(const sf::Transform& Transform, const sf::VertexArray& Vertex, const sf::Vector2f& point){
        Util::Triangle CurrentTriangle;

        for(size_t i = 0; i < Vertex.getVertexCount(); i += 3){
            CurrentTriangle.a = Transform.transformPoint(Vertex[i].position);
            CurrentTriangle.b = Transform.transformPoint(Vertex[i + 1].position);
            CurrentTriangle.c = Transform.transformPoint(Vertex[i + 2].position);
            if(Util::isInTriangle(CurrentTriangle, point)) {
                return true;
            }
        }

        return false;
    }

    bool RenderComponent::isColliding(Scriptable::Components::RenderComponent* other){
        // check if bounding boxes collide first (it's faster)
        if(!boundingBoxCollide(other)) return false;

        Util::Triangle CurrentTriangle;
        auto& Transform = getTransform();
        auto& OtherTransform = other->getTransform();
        for(size_t i = 0; i < other->m_verticesCount; i++){
            if(isPointInside_fast(Transform, m_vertices, OtherTransform.transformPoint(other->m_vertices[i].position))) return true;
        }
        for(size_t i = 0; i < m_verticesCount; i++){
            if(isPointInside_fast(OtherTransform, other->m_vertices, Transform.transformPoint(m_vertices[i].position))) return true;
        }

        return false;

    }

    bool RenderComponent::isPointInside(sf::Vector2f point){
        Util::Triangle CurrentTriangle;
        auto& Transform = getTransform();


        for(size_t i = 0; i < m_verticesCount; i += 3){
            CurrentTriangle.a = Transform.transformPoint(m_vertices[i].position);
            CurrentTriangle.b = Transform.transformPoint(m_vertices[i + 1].position);
            CurrentTriangle.c = Transform.transformPoint(m_vertices[i + 2].position);
            if(Util::isInTriangle(CurrentTriangle, point)) {
                return true;
            }
        }

        return false;
    }


    // checks if two AABBs collide
    bool RenderComponent::boundingBoxCollide(Scriptable::Components::RenderComponent* renderComponent) {
        sf::FloatRect boundingBox1 = getBoundingBox();
        sf::FloatRect boundingBox2 = renderComponent->getBoundingBox();

        sf::Vector2f position1 = getPosition();
        sf::Vector2f position2 = renderComponent->getPosition();

        // adjust the position of bounding boxes (the renderComponent position refers to it's center position not top left corner)
        boundingBox1.left = position1.x - boundingBox1.width / 2;
        boundingBox1.top = position1.y - boundingBox1.height / 2;

        boundingBox2.left = position2.x - boundingBox2.width / 2;
        boundingBox2.top = position2.y - boundingBox2.height / 2;

        return boundingBox1.intersects(boundingBox2);
    }

    sf::VertexArray RenderComponent::getVertices() {
        return m_vertices;
    }
}