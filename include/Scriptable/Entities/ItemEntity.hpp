#ifndef ITEM_ENTITY
#define ITEM_ENTITY

#include <Scriptable/Entity.hpp>

namespace Scriptable::Entities {

    class ItemEntity : public Scriptable::Entity {

        static const std::vector<sf::Vector2f> STEEL_VERTICES;
        static const std::vector<sf::Vector2f> ELECTRONIC_COMPONENTS_VERTICES;

        static const int MIN_ALPHA;
        static const int MAX_ALPHA;
        static const int ALPHA_CHANGE; // per second

        float m_alpha = 0;
        bool m_alphaIncreasing = false;

    public:
        enum Item {
            steel,
            electronic_components
        };

        Item itemType;

        ItemEntity(Item itemType, sf::Vector2f position);
        virtual ~ItemEntity() = default;
        
        void beforeRender(const Scriptable::EventData* data);
    };
}

#endif
