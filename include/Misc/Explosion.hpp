#ifndef __EXPLOSION
#define __EXPLOSION

#include <SFML/System.hpp>

namespace Scipp::ExplosionPresets {
    enum BulletHit {
        count = 4,
        size = 15,
        speed = 2,
        duration = 200,
    };
}

void explosion(sf::Vector2f position, int particleCount, float particleSize, float particleSpeed, float duration /*in milliseconds*/);

#endif