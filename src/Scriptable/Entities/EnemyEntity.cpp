#include <Scriptable/Entities/EnemyEntity.hpp>

#include <Scriptable/Components/RenderComponent.hpp>
#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Scriptable/Components/EnemyComponent.hpp>

namespace Scriptable::Entities {

	const std::map<EnemyEntity::Type, EnemyTypeInfo> EnemyEntity::TYPE_INFO = {
		{
			EnemyEntity::Type::normal,
			{
				{
					{25,25}, {25,50}, {10,40}, 
					{25,25}, {0,25}, {10,40},
					{25,25}, {0,25}, {10,10},
					{25,25}, {25,0}, {10,10},
					{25,25}, {25,0}, {40,10},
					{25,25}, {50,25}, {40,10},
					{25,25}, {50,25}, {40,40},
					{25,25}, {25,50}, {40,40}
				},
				100, // health
				150, // speed
				20, // damage
				{139, 204, 75}
			}
		},
		{
			EnemyEntity::Type::speedy,
			{
				{
					{25,25}, {25,50}, {10,40}, 
					{25,25}, {0,25}, {10,40},
					{25,25}, {0,25}, {10,10},
					{25,25}, {25,0}, {10,10},
					{25,25}, {25,0}, {40,10},
					{25,25}, {50,25}, {40,10},
					{25,25}, {50,25}, {40,40},
					{25,25}, {25,50}, {40,40}
				},
				50,
				200,
				20,
				{98, 186, 110}
			}
		},
		{
			EnemyEntity::Type::tank,
			{
				{
					{37.5f, 37.5f}, {37.5f, 75.0f}, {15.0f, 60.0f},
					{37.5f, 37.5f}, {0.0f, 37.5f}, {15.0f, 60.0f},
					{37.5f, 37.5f}, {0.0f, 37.5f}, {15.0f, 15.0f},
					{37.5f, 37.5f}, {37.5f, 0.0f}, {15.0f, 15.0f},
					{37.5f, 37.5f}, {37.5f, 0.0f}, {60.0f, 15.0f},
					{37.5f, 37.5f}, {75.0f, 37.5f}, {60.0f, 15.0f},
					{37.5f, 37.5f}, {75.0f, 37.5f}, {60.0f, 60.0f},
					{37.5f, 37.5f}, {37.5f, 75.0f}, {60.0f, 60.0f}
				},
				300,
				125,
				20,
				{183, 211, 107}
			}
		},
		{
			EnemyEntity::Type::boss,
			{
				{
					{100,100}, {100,200}, {40,160}, 
					{100,100}, {0,100}, {40,160},
					{100,100}, {0,100}, {40,40},
					{100,100}, {100,0}, {40,40},
					{100,100}, {100,0}, {160,40},
					{100,100}, {200,100}, {160,40},
					{100,100}, {200,100}, {160,160},
					{100,100}, {100,200}, {160,160}
				},
				2000,
				100,
				200,
				{178, 155, 62}
			}
		}
	};

	void EnemyEntity::randomItemDrop() {
		short random = rand() % 3 + 1;

		static uint32_t item_id = 0;

		if(random == 1) {
			auto* rc = getComponent<Scriptable::Components::RenderComponent>();
			Scipp::globalGame->stateManager.currentState->addEntity<ItemEntity>("item_" + item_id, ItemEntity::Item::steel, rc->getPosition());
		}
		else if(random == 2) {
			auto* rc = getComponent<Scriptable::Components::RenderComponent>();
			Scipp::globalGame->stateManager.currentState->addEntity<ItemEntity>("item_" + item_id, ItemEntity::Item::electronic_components, rc->getPosition());
		}

		++item_id;
	}

    EnemyEntity::EnemyEntity(sf::Vector2f pos, Type type, float healthMultiplier=1) {

		auto vertices = TYPE_INFO.at(type).vertices;
		auto speed = TYPE_INFO.at(type).speed;
		auto health = TYPE_INFO.at(type).health;
		auto color = TYPE_INFO.at(type).color;

        addComponent<Scriptable::Components::RenderComponent>(vertices);
		addComponent<Scriptable::Components::PhysicsComponent>(getComponent<Scriptable::Components::RenderComponent>());
		addComponent<Scriptable::Components::EnemyComponent>(speed);
		addComponent<Scriptable::Components::HealthComponent>(health*healthMultiplier);

		auto* rc = getComponent<Scriptable::Components::RenderComponent>();
		auto* hc = getComponent<Scriptable::Components::HealthComponent>();

		rc->setPosition(pos);
		rc->setOrigin(getComponent<Scriptable::Components::RenderComponent>()->center());
		rc->setColor(color);
		hc->setOnDeathCallback(deleteEnemyCallback);

		Scipp::globalGame->stateManager.currentState->addEntityToGroup(this, groupName);

		zindex = 2;
    }
}