#include <Scriptable/Entities/HealthbarEntity.hpp>

#include <iostream>
#include <cmath>

namespace Scriptable::Entities {
    HealthbarBackgroundEntity::HealthbarBackgroundEntity(std::vector<sf::Vector2f> vertices) {
        zindex = 4;

        addComponent<Scriptable::Components::RenderComponent>(vertices);

        auto* rc = getComponent<Scriptable::Components::RenderComponent>();
        rc->setOrigin(rc->center());
        rc->setColor(sf::Color(255, 50, 50));
    }

    HealthbarEntity::HealthbarEntity(std::string name, Scriptable::Entity* entity) {
        if(!entity->hasComponent<Scriptable::Components::RenderComponent>() || !entity->hasComponent<Scriptable::Components::HealthComponent>()) {
            std::clog << "Entity provided to a HealthbarEntity doesn't have a RenderComponent or a HealthComponent" << "\n";
            m_toBeDeleted = true; // we have to schedule the deletion because the entity doesn'y have a name assigned yet at this point (it's assigned after the constructor)
        }
        else {
            zindex = 5;

            Scipp::globalGame->stateManager.currentState->addEntity<HealthbarBackgroundEntity>(name + "background",m_vertices);
            m_backgroundEntity = (HealthbarBackgroundEntity*) Scipp::globalGame->stateManager.currentState->getEntity(name + "background");

            m_trackedRenderComponent = entity->getComponent<Scriptable::Components::RenderComponent>();
            m_trackedHealthComponent = entity->getComponent<Scriptable::Components::HealthComponent>();

            addComponent<Scriptable::Components::RenderComponent>(m_vertices);

            auto* rc = getComponent<Scriptable::Components::RenderComponent>();
            rc->setOrigin(rc->center());
            rc->setColor(sf::Color(100, 225, 50));
        }
    }

    void HealthbarEntity::update() {
        float healthWidth = (std::round(m_trackedHealthComponent->getHealth()) / std::round(m_trackedHealthComponent->getMaxHealth())) * m_healthbarWidth;
        
        auto* rc = getComponent<Scriptable::Components::RenderComponent>();
        auto* brc = m_backgroundEntity->getComponent<Scriptable::Components::RenderComponent>();

        auto rotation = m_trackedRenderComponent->getRotation()-90;
        auto position = Util::movePoint(m_trackedRenderComponent->getPosition(), m_offset, rotation-90);

        rc->setPosition(position);
        rc->setRotation(rotation);
        brc->setPosition(position);
        brc->setRotation(rotation);

        if(healthWidth != m_currentHealthbarWidth) {
            std::vector<sf::Vector2f> vertices = {
                {0,0}, {healthWidth, 0}, {0, m_healthbarHeight},
                {0, m_healthbarHeight}, {healthWidth, m_healthbarHeight}, {healthWidth, 0},
            };

            rc->setVertices(vertices);
        }
    }

    void HealthbarEntity::beforeRender(const EventData* data) {
        if(m_toBeDeleted) {
            Scipp::globalGame->stateManager.currentState->softDeleteEntity(getName());

            if(m_backgroundEntity != nullptr) {
                Scipp::globalGame->stateManager.currentState->softDeleteEntity(m_backgroundEntity->getName());
            }
        }
        else {
            float health = m_trackedHealthComponent->getHealth();
            if(health <= 0) {
                m_toBeDeleted = true;
            }
            else {
                update();
            }
        }
    }
}