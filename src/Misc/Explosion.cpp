#include <Misc/Explosion.hpp>

#include <Scriptable/Entities/ParticleEntity.hpp>

#include <iostream>

#include <Game.hpp>

void explosion(sf::Vector2f position, int particleCount, float particleSize, float particleSpeed, float duration) {
    static int id = 0;

    for(int i = 0; i < particleCount; i++) {
        float randomRotation = rand() % 360;

        Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::ParticleEntity>("particle" + id, position, randomRotation, particleSpeed, particleSize, sf::milliseconds(duration));
        id++;
    }
}