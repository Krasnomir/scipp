#ifndef ITEM_ENTITY
#define ITEM_ENTITY

#include <Scriptable/Entity.hpp>

#include <Scriptable/Components/LifetimeComponent.hpp>

#include <Game.hpp>

namespace Scriptable::Entities {

    class ItemEntity : public Scriptable::Entity {
    public:
        enum Item {
            steel,
            electronic_components,
            essence,
            null // this is used for dropping items on enemy death
        };
    private:
        static const std::vector<sf::Vector2f> STEEL_VERTICES;
        static const std::vector<sf::Vector2f> ELECTRONIC_COMPONENTS_VERTICES;
        static const std::vector<sf::Vector2f> ESSENCE_VERTICES;

        static const std::unordered_map<Item, sf::Color> ITEM_COLORS;

        static const sf::Time ITEM_LIFETIME;

        static const int MIN_ALPHA;
        static const int MAX_ALPHA;
        static const int ALPHA_CHANGE; // per second

        float m_alpha = 0;
        bool m_alphaIncreasing = false;

        static void deleteItemCallback(Scriptable::Components::LifetimeComponent* c) {
            Scipp::globalGame->stateManager.currentState->softDeleteEntity(((Scriptable::Entity*)c->parentEntity)->getName());
        }

    public:

        Item itemType;

        ItemEntity(Item itemType, sf::Vector2f position);
        virtual ~ItemEntity() = default;
        
        void beforeRender(const Scriptable::EventData* data);
    };
}

#endif
