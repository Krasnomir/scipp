#ifndef __HEALTHBAR_ENTITY
#define __HEALTHBAR_ENTITY

#include <Scriptable/Entity.hpp>

#include <Scriptable/Components/RenderComponent.hpp>
#include <Scriptable/Components/HealthComponent.hpp>

#include <Game.hpp>

namespace Scriptable::Entities {

    class HealthbarBackgroundEntity : public Scriptable::Entity 
    {
    public:
        HealthbarBackgroundEntity(std::vector<sf::Vector2f> vertices);
        virtual ~HealthbarBackgroundEntity() = default;
    };

    class HealthbarEntity : public Scriptable::Entity
    {
        bool m_toBeDeleted = false;

        float m_trackedEntityHalfWidth = 0; // for offseting healthbars from the tracked entity's center

        float m_currentHealthbarWidth = HEALTHBAR_WIDTH; // width of the green bar that indicates the ammount of health (over the red background bar that has static width)

        std::vector<sf::Vector2f> m_vertices = {
            {0,0}, {(float)HEALTHBAR_WIDTH, 0}, {0, (float)HEALTHBAR_HEIGHT},
            {0, (float)HEALTHBAR_HEIGHT}, {(float)HEALTHBAR_WIDTH, (float)HEALTHBAR_HEIGHT}, {(float)HEALTHBAR_WIDTH, 0}
        };

        HealthbarBackgroundEntity* m_backgroundEntity = nullptr;

        Scriptable::Components::RenderComponent* m_trackedRenderComponent = nullptr;
        Scriptable::Components::HealthComponent* m_trackedHealthComponent = nullptr;

    public:

        static const int HEALTHBAR_OPACITY;
        static const int HEALTHBAR_OFFSET;
        static const int HEALTHBAR_WIDTH;
        static const int HEALTHBAR_HEIGHT;

        HealthbarEntity(std::string name, Scriptable::Entity* entity);
        virtual ~HealthbarEntity() = default;

        void update();

        void beforeRender(const EventData* data);
    };
}

#endif
