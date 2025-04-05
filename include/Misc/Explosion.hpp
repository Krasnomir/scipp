#ifndef __EXPLOSION
#define __EXPLOSION

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace Scipp::ExplosionPresets {
    enum BulletHit {
        count = 4,
        size = 20,
        speed = 2,
        duration = 200,
        colorR = 164,
        colorG = 49,
        colorB = 34
    };
}

void explosion(sf::Vector2f position, int particleCount, float particleSize, float particleSpeed, float duration /*in milliseconds*/, sf::Color color);

#endif