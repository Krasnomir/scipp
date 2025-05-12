#include <Scriptable/Entities/EnemyEntity.hpp>

#include <Scriptable/Components/RenderComponent.hpp>
#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Scriptable/Components/EnemyComponent.hpp>

namespace Scriptable::Entities {

	const std::map<EnemyEntity::Type, EnemyEntity::item_drop_info> EnemyEntity::TYPE_DROP_INFO {
		{ 
			EnemyEntity::Type::normal,
			{
				{ ItemEntity::Item::steel, 1 },
				{ ItemEntity::Item::electronic_components, 1 },
				{ ItemEntity::Item::null, 2 }
			}
		},
		{ 
			EnemyEntity::Type::speedy,
			{
				{ ItemEntity::Item::electronic_components, 1 },
				{ ItemEntity::Item::null, 1 }
			}
		},
		{ 
			EnemyEntity::Type::tank,
			{
				{ ItemEntity::Item::steel, 1 },
				{ ItemEntity::Item::null, 1 }
			}
		},
		{ 
			EnemyEntity::Type::boss,
			{
				{ ItemEntity::Item::essence, 1 }
			}
		},
	};

	const std::map<EnemyEntity::Type, EnemyTypeInfo> EnemyEntity::TYPE_INFO = {
		{
			EnemyEntity::Type::normal,
			{
				{
					{0,0},{50,0},{50,50},
            		{0,0},{50,50},{0,50}
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
					{0,0},{40,0},{40,40},
            		{0,0},{40,40},{0,40}
				},
				80,
				200,
				20,
				{98, 186, 110}
			}
		},
		{
			EnemyEntity::Type::tank,
			{
				{
					{0,0},{60,0},{60,60},
            		{0,0},{60,60},{0,60}
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
					{0,0},{120,0},{120,120},
            		{0,0},{120,120},{0,120}
				},
				1000,
				100,
				200,
				{178, 155, 62}
			}
		}
	};

	void EnemyEntity::drop_item() {
		// drop info is stored as follows:
		// item_type, probability
		// so for example if there were two entries in the drop info map:
		// ITEM1, 9
		// ITEM2, 1
		// there would be 90% chance of dropping ITEM1, and 10% chance of dropping ITEM2

		auto drop_info = TYPE_DROP_INFO.at(m_type);

		int total = 0;
		for(const auto& item_info : drop_info) {
			total += item_info.second;
		}

		if(total == 0) return;

		int random = rand() % total + 1;

		auto* rc = getComponent<Scriptable::Components::RenderComponent>();

		int i = 0;
		for(const auto& item_info : drop_info) {
			i += item_info.second;
			if(random <= i) {
				if(item_info.first == ItemEntity::Item::null) break;

				static uint32_t item_id = 0;

				Scipp::globalGame->stateManager.currentState->addEntity<ItemEntity>("item_" + item_id, item_info.first, rc->getPosition());

				++item_id;
				break;
			}
		}
		/*
		short random = rand() % 3 + 1;

		if(random == 1) {
			auto* rc = getComponent<Scriptable::Components::RenderComponent>();
			Scipp::globalGame->stateManager.currentState->addEntity<ItemEntity>("item_" + item_id, ItemEntity::Item::steel, rc->getPosition());
		}
		else if(random == 2) {
			auto* rc = getComponent<Scriptable::Components::RenderComponent>();
			Scipp::globalGame->stateManager.currentState->addEntity<ItemEntity>("item_" + item_id, ItemEntity::Item::electronic_components, rc->getPosition());
		}
		*/
	}

    EnemyEntity::EnemyEntity(sf::Vector2f pos, Type type, float healthMultiplier=1) {

		m_type = type;

		auto vertices = TYPE_INFO.at(type).vertices;
		auto speed = TYPE_INFO.at(type).speed;
		auto health = TYPE_INFO.at(type).health;
		auto color = TYPE_INFO.at(type).color;

        addComponent<Scriptable::Components::RenderComponent>(vertices);
		addComponent<Scriptable::Components::PhysicsComponent>(getComponent<Scriptable::Components::RenderComponent>());
		addComponent<Scriptable::Components::EnemyComponent>(speed, vertices);
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