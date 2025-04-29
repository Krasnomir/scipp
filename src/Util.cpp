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
    
    bool isInTriangle(const Triangle& triangle, const sf::Vector2f& point)
    {
        auto sign = [](const sf::Vector2f& p1, const sf::Vector2f& p2, const sf::Vector2f& p3) {
            return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
        };

        float d1, d2, d3;
    
        d1 = sign(point, triangle.a, triangle.b);
        d2 = sign(point, triangle.b, triangle.c);
        d3 = sign(point, triangle.c, triangle.a);
    
        bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
        bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
    
        return !(has_neg && has_pos);
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

    float vec_magnitude(const sf::Vector2f& vec) {
        return sqrt(vec.x * vec.x + vec.y * vec.y);
    }

    sf::Vector2f vec_normalize(sf::Vector2f vec) {
        float len = vec_magnitude(vec);

        if(len == 0)
            return sf::Vector2f(0,0);
        
        return sf::Vector2f(vec.x / len, vec.y / len);
    }

    sf::Vector2f vec_from_mag_and_dir(float magnitude, float direction) {
        return sf::Vector2f(cosf(Util::deg_to_rad(direction)) * magnitude, sinf(Util::deg_to_rad(direction)) * magnitude);
    }

    float deg_to_rad(float angle) {
        static constexpr float DEG_TO_RAD = 3.14159265f / 180.0f;
        return angle * DEG_TO_RAD;
    }

    float rad_to_deg(float angle) {
        static constexpr float DEG_TO_RAD = 3.14159265f / 180.0f;
        return angle / DEG_TO_RAD;
    }
}