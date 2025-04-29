#ifndef PLAYER_ENTITY
#define PLAYER_ENTITY

#include <Scriptable/Entity.hpp>
#include <Scriptable/Entities/ItemEntity.hpp>

#include <Scriptable/Components/HealthComponent.hpp>

#include <GameState.hpp>
#include <DeathState.hpp>

#include <Game.hpp>

#include <SFML/System.hpp>

#include <vector>

#define PLAYER_SPEED 150

namespace Scriptable::Entities {

    class PlayerEntity : public Scriptable::Entity {

        static const sf::Time GUN_COOLDOWN;
        static const int GUN_DAMAGE;
        
        // --------------------
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
        std::map<m_dummy_type, std::unordered_map<ItemEntity::Item, int>> m_dummy_recipes {
            {m_dummy_type::turret,  {
                { ItemEntity::Item::steel, 1 },
                { ItemEntity::Item::electronic_components, 3 }
            }},
            {m_dummy_type::mud_trap, {
                { ItemEntity::Item::steel, 1 }
            }},
            {m_dummy_type::spike_trap, {
                { ItemEntity::Item::steel, 2 }
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

        bool pay(std::unordered_map<ItemEntity::Item, int> requiredItems);

        // --------------------

        std::unordered_map<ItemEntity::Item, int> m_inventory;

        float health = 100;
        float regenPerSecond = 10;
        float regenDelaySeconds = 5;
        float bulletDistance = 50;

        sf::Time m_bulletCooldown = sf::seconds(0);

        // dashing system variables
        bool m_isDashing = false;
        sf::Time m_dashDuration = sf::seconds(0.2);
        sf::Time m_dashCooldown = sf::seconds(5);
        sf::Time m_dashDurationTrack = sf::seconds(0);
        sf::Time m_dashCooldownTrack = sf::seconds(0);
        float m_dashSpeed = 2000;
        sf::Vector2f m_dash_vec;

        std::vector<sf::Vector2f> vertices = {
            {0,0},{40,0},{40,40},
            {0,0},{40,40},{0,40}
        };

        static void playerDeathCallback(Scriptable::Components::HealthComponent* c) {
            Scipp::globalGame->stateManager.scheduleStateChange(new DeathState());
        }

public:
        virtual ~PlayerEntity() = default;
        PlayerEntity();

        void beforeRender(const Scriptable::EventData* data);
        void onKeyPressed(const Scriptable::EventData* data);
        void onMouseMoved(const Scriptable::EventData* data);
        void onMouseButtonPressed(const Scriptable::EventData* data);
    
        void handleGun(const Scriptable::EventData* data);
        void requestFiring();

        // placement system
        void requestDummy(m_dummy_type type);
        void handleDummy();
        void cancelDummy();
        void requestPlacement(const Scriptable::EventData* data);

        void handleDash(const Scriptable::EventData* data);
        void dash();

        void handleItems(const Scriptable::EventData* data);

        void handleMovement();
    };
    
}

#endif
