#pragma once
#include <SFML/Graphics.hpp>

#include <cmath>
#include <random>
#include <filesystem>
#include <sstream>


// [WARNING]: functions in ::Util may be thread-unsafe
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

    bool setResourceDirectory(const std::filesystem::path& path);

    //returns resourceDirectory + resPath
    std::filesystem::path getPathToResource(const std::string& resPath);

    template <typename T>
    T numToAngle(T num) {
        if(num > 0) return std::fmod((float)num, 360);
        return 360 + std::fmod((float)num, 360);
    }

    typedef std::vector<sf::Vector2f> shape_t;
    shape_t CreateRectangle(sf::FloatRect rect);

    float vec_magnitude(const sf::Vector2f& vec);
    sf::Vector2f vec_normalize(sf::Vector2f vec);
    sf::Vector2f vec_from_mag_and_dir(float mag, float direction); // direction in radians

    float deg_to_rad(float angle);
    float rad_to_deg(float angle);
}