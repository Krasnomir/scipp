#ifndef __RENDER_COMPONENT
#define __RENDER_COMPONENT 1

#include <Scriptable/Component.hpp>
#include <SFML/Graphics.hpp>
#include <Util.hpp>

#include <string>
#include <vector>
#include <unordered_map> 
#include <math.h>

namespace Scriptable::Components{

    struct RotationMatrix2x2 {
        float m[2][2];

        // Constructor: Initialize the matrix to represent an identity matrix by default
        RotationMatrix2x2() {
            m[0][0] = m[1][1] = 1.0f;
            m[0][1] = m[1][0] = 0.0f;
        }

        // Constructor to initialize the matrix based on an angle (in radians)
        RotationMatrix2x2(float cosA, float sinA) {
            m[0][0] = cosA;
            m[0][1] = sinA;
            m[1][0] = -sinA;
            m[1][1] = cosA;
        }

        // Rotate a 2D vector using the matrix
        sf::Vector2f rotate(const sf::Vector2f& v) const {
            return sf::Vector2f(
                m[0][0] * v.x + m[0][1] * v.y,
                m[1][0] * v.x + m[1][1] * v.y
            );
        }
    };

    class RenderComponent : public Component, public sf::Drawable, public sf::Transformable {
    public:
        RenderComponent() = delete;

        //WARNING: This function requires input in the format of SFML triangles
        RenderComponent(const Util::shape_t& trianglePoints);

        //WARNING: This function requires input in the format of pairs of {triangle_vertex : txt_coord};
        RenderComponent(const std::vector<std::pair<sf::Vector2f, sf::Vector2f>>& trianglePointsTxtCoords);

        virtual ~RenderComponent() = default;

        void setVertices(const std::vector<sf::Vector2f>& points);

        sf::Vector2f center();

        void setRotation(float angle);
        void setRotation(double angle);

        void onRender(const EventData* data);

        void addCostume(std::string name, std::string path, sf::IntRect area);
        void addCostume(std::string name, std::string path);
        void loadCostume(std::string name);

        // both setAlpha() and setColor() apply the color values to all vertices
        void setAlpha(float alpha);
        int getAlpha();

        void setColor(sf::Color color);
        void setColor(sf::Color color, int index);
        sf::Color getColor();

        // void rotationIndependentBoundingBox();
        void AABB();

        void boundingBoxInit();
        sf::FloatRect getBoundingBox();
        bool boundingBoxCollide(Scriptable::Components::RenderComponent* renderComponent);

        bool isPointInside(sf::Vector2f point);

        bool isColliding(Scriptable::Components::RenderComponent* other);

        sf::VertexArray getVertices();

    private:
        int m_verticesCount;

        sf::FloatRect m_boundingBox;

        sf::VertexArray m_vertices;
        sf::Texture m_texture;

        int m_alpha = 255;
        sf::Color m_color = {255, 255, 255};

        mutable std::shared_mutex m_costumesLock;
        std::unordered_map<std::string, sf::Texture> m_costumes;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        Components::RotationMatrix2x2 m_rotationMatrix;
    };
}

#endif