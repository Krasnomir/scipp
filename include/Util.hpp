#pragma once
#include <SFML/Graphics.hpp>

#include <cmath>

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

    template <typename T>
    T numToAngle(T num) {
        if(num > 0) return std::fmod((float)num, 360);
        return 360 + std::fmod((float)num, 360);
    }

    typedef std::vector<sf::Vector2f> shape_t;
    shape_t CreateRectangle(sf::FloatRect rect);
}