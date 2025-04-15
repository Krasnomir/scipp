#define _USE_MATH_DEFINES
#include <Util.hpp>

#include <SFML/Graphics.hpp>

#include <math.h>

namespace Util {
    std::filesystem::path resourceDirectory = "./res/";
    
    bool setResourceDirectory(const std::filesystem::path& path){
        if(!std::filesystem::is_directory(path)){
            return false;
        }

        resourceDirectory = path;
        return true;
    }

    //returns resourceDirectory + resPath
    std::filesystem::path getPathToResource(const std::string& resPath){
        return (resourceDirectory / resPath); 
    }

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

    float getTriangleArea(const Triangle& t) {
        return abs((t.a.x * (t.b.y - t.c.y) + t.b.x * (t.c.y - t.a.y) + t.c.x * (t.a.y - t.b.y)) / 2.0);
    }

    bool isInTriangle(const Triangle& triangle, const sf::Vector2f& point) {
        float abc = Util::getTriangleArea(triangle);
        float pbc = Util::getTriangleArea(Triangle(point, triangle.b, triangle.c));
        float pac = Util::getTriangleArea(Triangle(point, triangle.a, triangle.c));
        float pab = Util::getTriangleArea(Triangle(point, triangle.a, triangle.b));

        return fabs(abc - pab - pac - pbc) < 0.001;
    }

    shape_t CreateRectangle(sf::FloatRect rect){
        Triangle a,b;

        //Center is set to 0,0

        /*
        left,top -> is the center
        */

        float halfWidth = rect.width / 2.f;
        float halfHeight = rect.height / 2.f;

        a.a = {-halfWidth, -halfHeight};
        a.b = {-halfWidth, halfHeight};
        a.c = {halfWidth, halfHeight};

        b.a = a.c;
        b.b = a.a;
        b.c = {halfWidth, -halfHeight}; 

        return std::vector<sf::Vector2f>{a.a, a.b, a.c, b.a, b.b, b.c};
    }


}