#pragma once
#include <SFML/Graphics.hpp>

namespace Util {
    struct Triangle {
        sf::Vector2f a;
        sf::Vector2f b;
        sf::Vector2f c;
    };

    sf::Vector2f movePoint(const sf::Vector2f& position, double distance, double angleDegrees);
    float getAngleBetweenPoints(const sf::Vector2f& position1, const sf::Vector2f& position2);
    float getDistanceBetweenPoints(const sf::Vector2f& position1, const sf::Vector2f& position2);
    float getTriangleArea(const Triangle& t);
    bool isInTriangle(const Triangle& triangle, const sf::Vector2f& point);

    typedef std::vector<sf::Vector2f> shape_t;
    shape_t CreateRectangle(sf::FloatRect rect);
}