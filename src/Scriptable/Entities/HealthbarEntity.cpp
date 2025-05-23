#include <Scriptable/Entities/HealthbarEntity.hpp>

#include <iostream>
#include <cmath>
#include <limits>

namespace Scriptable::Entities {

    const int HealthbarEntity::HEALTHBAR_OPACITY = 150;
    const int HealthbarEntity::HEALTHBAR_OFFSET = 20;
    const int HealthbarEntity::HEALTHBAR_WIDTH = 50;
    const int HealthbarEntity::HEALTHBAR_HEIGHT = 10;

    HealthbarBackgroundEntity::HealthbarBackgroundEntity(std::vector<sf::Vector2f> vertices, sf::Vector2f pos) {
        zindex = 4;

        addComponent<Scriptable::Components::RenderComponent>(vertices);

        auto* rc = getComponent<Scriptable::Components::RenderComponent>();
        rc->setOrigin(rc->center());
        rc->setColor(sf::Color(255, 50, 50, HealthbarEntity::HEALTHBAR_OPACITY));
        rc->setPosition(pos);
    }

    HealthbarEntity::HealthbarEntity(std::string name, Scriptable::Entity* entity) {
        if(!entity->hasComponent<Scriptable::Components::RenderComponent>() || !entity->hasComponent<Scriptable::Components::HealthComponent>()) {
            std::clog << "Entity provided to a HealthbarEntity doesn't have a RenderComponent or a HealthComponent" << "\n";
            m_toBeDeleted = true; // we have to schedule the deletion because the entity doesn'y have a name assigned yet at this point (it's assigned after the constructor)
        }
        else {
            zindex = 5;

            m_trackedRenderComponent = entity->getComponent<Scriptable::Components::RenderComponent>();
            m_trackedHealthComponent = entity->getComponent<Scriptable::Components::HealthComponent>();

            Scipp::globalGame->stateManager.currentState->addEntity<HealthbarBackgroundEntity>(name + "background",m_vertices, m_trackedRenderComponent->getPosition());
            m_backgroundEntity = (HealthbarBackgroundEntity*) Scipp::globalGame->stateManager.currentState->getEntity(name + "background");

            addComponent<Scriptable::Components::RenderComponent>(m_vertices);

            auto* rc = getComponent<Scriptable::Components::RenderComponent>();
            rc->setOrigin(rc->center());
            rc->setColor(sf::Color(100, 225, 50, HEALTHBAR_OPACITY));
            rc->setPosition(m_trackedRenderComponent->getPosition());

            // finds the width of the tracked entity's RenderComponent
            float maxX = std::numeric_limits<float>::min();
            float minX = std::numeric_limits<float>::max();
            auto vertices = m_trackedRenderComponent->getVertices();
            for(size_t i = 0; i < vertices.getVertexCount(); i++) {
                if(vertices[i].position.x > maxX) maxX = vertices[i].position.x;
                if(vertices[i].position.x < minX) minX = vertices[i].position.x;
            }
            m_trackedEntityHalfWidth = (maxX - minX) / 2;
        }
    }

    void HealthbarEntity::update() {
        float healthWidth = (std::round(m_trackedHealthComponent->getHealth()) / std::round(m_trackedHealthComponent->getMaxHealth())) * HEALTHBAR_WIDTH;
        
        auto* rc = getComponent<Scriptable::Components::RenderComponent>();
        auto* brc = m_backgroundEntity->getComponent<Scriptable::Components::RenderComponent>();

        auto rotation = m_trackedRenderComponent->getRotation()-90;
        auto position = Util::movePoint(m_trackedRenderComponent->getPosition(), HEALTHBAR_OFFSET + m_trackedEntityHalfWidth, rotation-90);

        rc->setPosition(position);
        rc->setRotation(rotation);
        brc->setPosition(position);
        brc->setRotation(rotation);

        if(healthWidth != m_currentHealthbarWidth) {
            std::vector<sf::Vector2f> vertices = {
                {0,0}, {healthWidth, 0}, {0, HEALTHBAR_HEIGHT},
                {0, HEALTHBAR_HEIGHT}, {healthWidth, HEALTHBAR_HEIGHT}, {healthWidth, 0},
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