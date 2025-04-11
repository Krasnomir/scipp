#ifndef __HEALTHBAR_ENTITY
#define __HEALTHBAR_ENTITY

#include <Scriptable/Entity.hpp>

#include <Scriptable/Components/RenderComponent.hpp>
#include <Scriptable/Components/HealthComponent.hpp>

#include <Game.hpp>

namespace Scriptable::Entities {

    static const int HEALTHBAR_OPACITY = 150;

    class HealthbarBackgroundEntity : public Scriptable::Entity 
    {
    public:
        HealthbarBackgroundEntity(std::vector<sf::Vector2f> vertices);
        virtual ~HealthbarBackgroundEntity() = default;
    };

    class HealthbarEntity : public Scriptable::Entity
    {
        bool m_toBeDeleted = false;

        float m_healthbarWidth = 50;
        float m_healthbarHeight = 10;
        float m_offset = 10;
        float m_trackedEntityHalfWidth = 0; // for offseting healthbars from the tracked entity's center

        float m_currentHealthbarWidth = m_healthbarWidth;

        std::vector<sf::Vector2f> m_vertices = {
            {0,0}, {(float)m_healthbarWidth, 0}, {0, (float)m_healthbarHeight},
            {0, (float)m_healthbarHeight}, {(float)m_healthbarWidth, (float)m_healthbarHeight}, {(float)m_healthbarWidth, 0}
        };

        HealthbarBackgroundEntity* m_backgroundEntity = nullptr;

        Scriptable::Components::RenderComponent* m_trackedRenderComponent = nullptr;
        Scriptable::Components::HealthComponent* m_trackedHealthComponent = nullptr;

    public:
        HealthbarEntity(std::string name, Scriptable::Entity* entity);
        virtual ~HealthbarEntity() = default;

        void update();

        void beforeRender(const EventData* data);
    };
}

#endif
