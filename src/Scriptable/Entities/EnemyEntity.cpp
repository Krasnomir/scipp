#include <Scriptable/Entities/EnemyEntity.hpp>

#include <Scriptable/Components/RenderComponent.hpp>
#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Scriptable/Components/EnemyComponent.hpp>

namespace Scriptable::Entities {
    EnemyEntity::EnemyEntity(sf::Vector2f pos) {
        addComponent<Scriptable::Components::RenderComponent>(vertices);
		addComponent<Scriptable::Components::PhysicsComponent>(getComponent<Scriptable::Components::RenderComponent>());
		addComponent<Scriptable::Components::EnemyComponent>();
		addComponent<Scriptable::Components::HealthComponent>(health);

		auto* rc = getComponent<Scriptable::Components::RenderComponent>();
		auto* hc = getComponent<Scriptable::Components::HealthComponent>();

		rc->setPosition(pos);
		rc->setOrigin(getComponent<Scriptable::Components::RenderComponent>()->center());
		rc->setColor(sf::Color(120, 177, 108));
		hc->setOnDeathCallback(deleteEnemyCallback);

		Scipp::globalGame->stateManager.currentState->addEntityToGroup(this, groupName);
    }
}