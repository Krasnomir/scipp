#ifndef __EXPLOSION
#define __EXPLOSION

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

void explosion(sf::Vector2f position, int particleCount, float particleSize, float particleSpeed, float duration /*in milliseconds*/, sf::Color color);

#endif