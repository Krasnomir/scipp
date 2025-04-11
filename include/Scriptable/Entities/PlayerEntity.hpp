#ifndef PLAYER_ENTITY
#define PLAYER_ENTITY

#include <Scriptable/Entity.hpp>

#include <SFML/System.hpp>

#include <vector>

namespace Scriptable::Entities {
    class PlayerEntity : public Scriptable::Entity {
        float health = 100;
        float regenPerSecond = 10;
        float regenDelaySeconds = 5;
        float bulletDistance = 50;

        // dashing system variables
        bool m_isDashing = false;
        sf::Time m_dashDuration = sf::seconds(0.25);
        sf::Time m_dashCooldown = sf::seconds(1);
        sf::Time m_dashDurationTrack = sf::seconds(0);
        sf::Time m_dashCooldownTrack = sf::seconds(0);
        float m_dashSpeed = 10;

        std::vector<std::pair<sf::Vector2f, sf::Vector2f>> vertices = {{{0,0}, {18,70}}, {{0, 100}, {18, 170}}, {{30, 0},  {48, 70}}, {{30,0}, {48, 70}}, {{30,100}, {48, 170}},{{0,100},{18, 170}}};

public:
        virtual ~PlayerEntity() = default;
        PlayerEntity();

        void beforeRender(const Scriptable::EventData* data);
        void onKeyPressed(const Scriptable::EventData* data);
        void onMouseMoved(const Scriptable::EventData* data);
        void onMouseButtonPressed(const Scriptable::EventData* data);
        void handleDash(const Scriptable::EventData* data);
        void dash();
    };
    
}

#endif
