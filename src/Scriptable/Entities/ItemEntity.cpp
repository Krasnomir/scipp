#include <Scriptable/Entities/ItemEntity.hpp>

#include <Scriptable/Components/RenderComponent.hpp>

#include <Game.hpp>

#include <iostream>

namespace Scriptable::Entities {
    const std::vector<sf::Vector2f> ItemEntity::STEEL_VERTICES = {
        {0.f,0.f}, {20.f, 0.f}, {20.f, 20.f},
        {0.f, 0.f}, {20.f, 20.f}, {0.f, 20.f}
    };

    const std::vector<sf::Vector2f> ItemEntity::ELECTRONIC_COMPONENTS_VERTICES = {
        {0.f,0.f}, {20.f, 0.f}, {20.f, 20.f},
        {0.f, 0.f}, {20.f, 20.f}, {0.f, 20.f}
    };

    const int ItemEntity::MIN_ALPHA = 155;
    const int ItemEntity::MAX_ALPHA = 255;
    const int ItemEntity::ALPHA_CHANGE = 300; // per second

    ItemEntity::ItemEntity(Item type, sf::Vector2f position) {
        std::vector<sf::Vector2f> vertices;

        itemType = type;

        switch (type) {
        case Item::steel:
            vertices = STEEL_VERTICES;
            break;
        case Item::electronic_components:
            vertices = ELECTRONIC_COMPONENTS_VERTICES;
            break;
        }

        addComponent<Scriptable::Components::RenderComponent>(vertices);

        auto* rc = getComponent<Scriptable::Components::RenderComponent>();

        rc->setOrigin(rc->center());
        rc->setPosition(position);

        m_alpha = MIN_ALPHA;

        Scipp::globalGame->stateManager.currentState->addEntityToGroup(this, "items");
    }

    void ItemEntity::beforeRender(const Scriptable::EventData* data) {

        auto* rc = getComponent<Scriptable::Components::RenderComponent>();

        float increment = data->deltaTime.asSeconds() * ALPHA_CHANGE;

        if(m_alphaIncreasing) {
            m_alpha += increment;
        }
        else {
            m_alpha -= increment;
        }

        if(m_alpha <= MIN_ALPHA) {
            m_alphaIncreasing = true;
            m_alpha = MIN_ALPHA;
        }
        else if(m_alpha >= MAX_ALPHA) {
            m_alphaIncreasing = false;
            m_alpha = MAX_ALPHA;
        }

        rc->setAlpha(m_alpha);
    }
}