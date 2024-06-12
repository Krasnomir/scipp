#define _USE_MATH_DEFINES

#include <Util.hpp>

#include <SFML/Graphics.hpp>

#include <math.h>

namespace Util {
	sf::Vector2f movePoint(const sf::Vector2f& position, double distance, double angleDegrees) {
        // Convert the angle from degrees to radians
        float angleRadians = angleDegrees * M_PI / 180.0;

        // Calculate the new coordinates
        float movedX = position.x + distance * cos(angleRadians);
        float movedY = position.y + distance * sin(angleRadians);

        return { sf::Vector2f(movedX, movedY) };
	}

    float getAngleBetweenPoints(const sf::Vector2f& position1, const sf::Vector2f& position2) {
        // Calculate the difference in x and y coordinates
        double deltaX = position2.x - position1.x;
        double deltaY = position2.y - position1.y;

        // Calculate the angle in radians
        double angleRadians = atan2(deltaY, deltaX);

        // Convert the angle to degrees
        double angleDegrees = angleRadians * (180.0 / M_PI);

        // Ensure the angle is in the range [0, 360)
        if (angleDegrees < 0) {
            angleDegrees += 360;
        }

        return angleDegrees;
    }

    float getDistanceBetweenPoints(const sf::Vector2f& position1, const sf::Vector2f& position2) {
        return sqrt(pow(position1.x - position2.x, 2) + pow((position1.y - position2.y), 2));
    }
}