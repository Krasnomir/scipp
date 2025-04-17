#ifndef PLAYER_ENTITY
#define PLAYER_ENTITY

#include <Scriptable/Entity.hpp>
#include <Scriptable/Entities/ItemEntity.hpp>

#include <SFML/System.hpp>

#include <vector>

namespace Scriptable::Entities {
    class PlayerEntity : public Scriptable::Entity {
        // PLACEMENT SYSTEM
        // defined in the .cpp file
        static const sf::Color    DUMMY_COLOR_ALLOWED;
        static const sf::Color    DUMMY_COLOR_FORBIDDEN;
        static const short        DUMMY_COLOR_ALPHA;
        static const short        DUMMY_ZINDEX;

        enum m_dummy_type {
            turret
        };
        std::map<std::string, std::vector<sf::Vector2f>> m_dummy_vertices {
            {"turret",  {
                {0,0}, {0,40}, {40,0}, 
                {0,40}, {40,40}, {40,0}, 
                {0,40}, {0,0}, {-10,20},
                {40,40}, {0,40}, {20,50},
                {40,40}, {40,0}, {70,20},
                {0,0}, {40,0}, {20,-10},
            }}
        };
        Entity* m_dummy = nullptr;
        bool m_hasDummy = false;
        bool m_dummyAllowed = false;

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

        std::map<ItemEntity::Item, int> m_inventory;

public:
        virtual ~PlayerEntity() = default;
        PlayerEntity();

        void beforeRender(const Scriptable::EventData* data);
        void onKeyPressed(const Scriptable::EventData* data);
        void onMouseMoved(const Scriptable::EventData* data);
        void onMouseButtonPressed(const Scriptable::EventData* data);
        
        // placement system
        void requestDummy(int type);
        void handleDummy();
        void cancelDummy();

        void handleDash(const Scriptable::EventData* data);
        void dash();

        void handleItems(const Scriptable::EventData* data);
    };
    
}

#endif
