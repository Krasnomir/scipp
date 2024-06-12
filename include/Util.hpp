#include <SFML/Graphics.hpp>

namespace Util {
    sf::Vector2f movePoint(const sf::Vector2f& position, double distance, double angleDegrees);
    float getAngleBetweenPoints(const sf::Vector2f& position1, const sf::Vector2f& position2);
    float getDistanceBetweenPoints(const sf::Vector2f& position1, const sf::Vector2f& position2);
}