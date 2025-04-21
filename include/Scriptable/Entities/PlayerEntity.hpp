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
            turret,
            mud_trap,
            spike_trap
        };
        std::map<m_dummy_type, std::vector<sf::Vector2f>> m_dummy_vertices {
            {m_dummy_type::turret,  {
                {0,0}, {0,40}, {40,0}, 
                {0,40}, {40,40}, {40,0}, 
                {0,40}, {0,0}, {-10,20},
                {40,40}, {0,40}, {20,50},
                {40,40}, {40,0}, {70,20},
                {0,0}, {40,0}, {20,-10},
            }},
            {m_dummy_type::mud_trap, {
                {0,0},{100,0},{0,100},
                {100,0},{100,100},{0,100}
            }},
            {m_dummy_type::spike_trap, {
                {0,0},{100,0},{0,100},
                {100,0},{100,100},{0,100}
            }}
        };
        std::vector<m_dummy_type> m_placementPanel = {
            m_dummy_type::turret,
            m_dummy_type::mud_trap,
            m_dummy_type::spike_trap
        };
        short m_placementPanelIndex = 0;
        Entity* m_dummy = nullptr;
        bool m_hasDummy = false;
        bool m_dummyAllowed = false;
        m_dummy_type m_currentDummyType = turret;

        float health = 100;
        float regenPerSecond = 10;
        float regenDelaySeconds = 5;
        float bulletDistance = 50;

        // dashing system variables
        bool m_isDashing = false;
        sf::Time m_dashDuration = sf::seconds(0.1);
        sf::Time m_dashCooldown = sf::seconds(2);
        sf::Time m_dashDurationTrack = sf::seconds(0);
        sf::Time m_dashCooldownTrack = sf::seconds(0);
        float m_dashSpeed = 20;

        std::vector<sf::Vector2f> vertices = {
            {25,25}, {25,50}, {10,40}, 
            {25,25}, {0,25}, {10,40},
            {25,25}, {0,25}, {10,10},
            {25,25}, {25,0}, {10,10},
            {25,25}, {25,0}, {40,10},
            {25,25}, {50,25}, {40,10},
            {25,25}, {50,25}, {40,40},
            {25,25}, {25,50}, {40,40}
        };

        std::map<ItemEntity::Item, int> m_inventory;

public:
        virtual ~PlayerEntity() = default;
        PlayerEntity();

        void beforeRender(const Scriptable::EventData* data);
        void onKeyPressed(const Scriptable::EventData* data);
        void onMouseMoved(const Scriptable::EventData* data);
        void onMouseButtonPressed(const Scriptable::EventData* data);
        
        // placement system
        void requestDummy(m_dummy_type type);
        void handleDummy();
        void cancelDummy();
        void requestPlacement(const Scriptable::EventData* data);

        void handleDash(const Scriptable::EventData* data);
        void dash();

        void handleItems(const Scriptable::EventData* data);
    };
    
}

#endif
