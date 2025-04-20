#include <Scriptable/Entities/PlayerEntity.hpp>

#include <Game.hpp>

#include <Scriptable/Components/RenderComponent.hpp>
#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Scriptable/Components/HealthComponent.hpp>
#include <Scriptable/Entities/TurretEntity.hpp>
#include <Scriptable/Entities/BulletEntity.hpp>
#include <Scriptable/Entities/HealthbarEntity.hpp>
#include <Scriptable/Entities/EnemyEntity.hpp>
#include <Scriptable/Entities/SimpleEntity.hpp>

#include <iostream>

namespace Scriptable::Entities {
	sf::Color const PlayerEntity::DUMMY_COLOR_ALLOWED 	= sf::Color(80, 210, 80);
	sf::Color const PlayerEntity::DUMMY_COLOR_FORBIDDEN = sf::Color(220, 70, 70);
	short const PlayerEntity::DUMMY_COLOR_ALPHA 		= 150;
	short const PlayerEntity::DUMMY_ZINDEX				= 10;

    PlayerEntity::PlayerEntity() {
        zindex = -1;

		addComponent<Scriptable::Components::RenderComponent>(vertices);
		addComponent<Scriptable::Components::PhysicsComponent>(getComponent<Scriptable::Components::RenderComponent>());
		addComponent<Scriptable::Components::HealthComponent>(health, health, regenPerSecond, regenDelaySeconds);

		getComponent<Scriptable::Components::RenderComponent>()->setOrigin(getComponent<Scriptable::Components::RenderComponent>()->center());
		//getComponent<Scriptable::Components::RenderComponent>()->addCostume("test", Util::getPathToResource("test.png"), sf::IntRect({0,0, 398, 273}));
		//getComponent<Scriptable::Components::RenderComponent>()->loadCostume("test");
		getComponent<Scriptable::Components::RenderComponent>()->setColor(sf::Color(62, 75, 76));

		getComponent<Scriptable::Components::PhysicsComponent>()->collidable = true;

		Scipp::globalGame->stateManager.currentState->addEntityToGroup(this, "friendly");
		Scipp::globalGame->stateManager.currentState->addEntityToGroup(this, "collidable");
    }

	void PlayerEntity::beforeRender(const Scriptable::EventData* data) {

		handleDash(data);
		handleDummy();

		handleItems(data);

		auto* rc = this->getComponent<Scriptable::Components::RenderComponent>();

		float dashValue = 0;
		if(m_isDashing) dashValue = m_dashSpeed;

		// wasd movement
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
			auto* pc = getComponent<Scriptable::Components::PhysicsComponent>();
			pc->velocity.magnitude = 5 + dashValue;
			pc->velocity.direction = rc->getRotation();
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
			auto* pc = getComponent<Scriptable::Components::PhysicsComponent>();
			pc->velocity.magnitude = 5 + dashValue;
			pc->velocity.direction = rc->getRotation() - 180;
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
			auto* pc = getComponent<Scriptable::Components::PhysicsComponent>();
			pc->velocity.magnitude = 5 + dashValue;
			pc->velocity.direction = rc->getRotation() - 90;
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			auto* pc = getComponent<Scriptable::Components::PhysicsComponent>();
			pc->velocity.magnitude = 5 + dashValue;
			pc->velocity.direction = rc->getRotation() + 90;
		}
		else {
			auto* pc = getComponent<Scriptable::Components::PhysicsComponent>();
			pc->velocity.magnitude = dashValue;
			pc->velocity.direction = rc->getRotation();
		}
	}

	void PlayerEntity::onMouseMoved(const Scriptable::EventData* data) {
		auto mousePos = Scipp::globalGame->stateManager.currentState->M_camera.getMousePositionRelativeToCamera(true);
		auto* renderComponent = getComponent<Scriptable::Components::RenderComponent>();


		renderComponent->setRotation(Util::getAngleBetweenPoints(renderComponent->getPosition(), mousePos));
	}

	void PlayerEntity::onMouseButtonPressed(const Scriptable::EventData* data) {
		if(data->sfmlEvent.mouseButton.button == sf::Mouse::Button::Right) {
			if(!(m_hasDummy && m_dummyAllowed)) return;

			if(m_inventory[ItemEntity::Item::steel] >= 1) {
				static uint32_t turret_ID = 0;

				auto* rc = m_dummy->getComponent<Scriptable::Components::RenderComponent>();

				data->currentState->addEntity<Scriptable::Entities::TurretEntity>("turret" + std::to_string(turret_ID), rc->getPosition());
				data->currentState->addEntity<Scriptable::Entities::HealthbarEntity>("healthbar_turret" + std::to_string(turret_ID), "healthbar_turret" + std::to_string(turret_ID), Scipp::globalGame->stateManager.currentState->getEntity("turret" + std::to_string(turret_ID)));
				turret_ID++;

				m_inventory[ItemEntity::Item::steel]--;
			}

			cancelDummy();
		}
		if(data->sfmlEvent.mouseButton.button == sf::Mouse::Button::Left) {
			static uint32_t proj_ID = 0;

			auto* rc = getComponent<Scriptable::Components::RenderComponent>();
			sf::Vector2f bulletStartPosition = Util::movePoint(rc->getPosition(), bulletDistance, rc->getRotation());

			Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::BulletEntity>(std::to_string(proj_ID), rc->getRotation(), bulletStartPosition, 20);

			proj_ID++;
		}
	}

	void PlayerEntity::onKeyPressed(const Scriptable::EventData* data) {
		if(data->sfmlEvent.key.scancode == sf::Keyboard::Scancode::E) {
			requestDummy(m_dummy_type::turret);
		}
		else if(data->sfmlEvent.key.scancode == sf::Keyboard::Scancode::Q) {
			static uint32_t enemy_ID = 0;

			auto* rc = getComponent<Scriptable::Components::RenderComponent>();
			sf::Vector2f enemyStartPosition = Util::movePoint(rc->getPosition(), 500, rc->getRotation());

			Scipp::globalGame->stateManager.currentState->addEntity<EnemyEntity>("enemy" + std::to_string(enemy_ID), enemyStartPosition, EnemyEntity::Type::boss);
			Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::HealthbarEntity>("healthbar_enemy" + std::to_string(enemy_ID), "healthbar_enemy" + std::to_string(enemy_ID), Scipp::globalGame->stateManager.currentState->getEntity("enemy" + std::to_string(enemy_ID)));
			enemy_ID++;
		}
		else if(data->sfmlEvent.key.scancode == sf::Keyboard::Scancode::F) {
			dash();
		}
	}

	void PlayerEntity::requestDummy(int type) {
		if(m_hasDummy) return;

		auto* currentState = Scipp::globalGame->stateManager.currentState;

		if(type == m_dummy_type::turret) {
			currentState->addEntity<Scriptable::Entities::SimpleEntity>("dummy", m_dummy_vertices["turret"]);
			m_dummy = currentState->getEntity("dummy");

			m_dummy->zindex = DUMMY_ZINDEX;

			auto* rc = m_dummy->getComponent<Scriptable::Components::RenderComponent>();

			rc->setAlpha(100);
			rc->setColor(DUMMY_COLOR_FORBIDDEN);
		}

		m_hasDummy = true;
	}

	void PlayerEntity::handleDummy() {
		if(m_dummy != nullptr) {
			auto* currentState = Scipp::globalGame->stateManager.currentState;
			auto* rc = m_dummy->getComponent<Scriptable::Components::RenderComponent>();

			// set the dummy's position to mouse position
			rc->setPosition(currentState->M_camera.getMousePositionRelativeToCamera(true));

			m_dummyAllowed = true;
			std::vector<Entity*> friendly = currentState->getEntitiesFromGroup("friendly");
			for(auto& friend1 : friendly) {
				if(friend1->getComponent<Scriptable::Components::RenderComponent>()->isColliding(rc)) m_dummyAllowed = false;
			}

			if(m_dummyAllowed) {
				rc->setColor(DUMMY_COLOR_ALLOWED);
			}
			else {
				rc->setColor(DUMMY_COLOR_FORBIDDEN);
			}
		}
	}

	void PlayerEntity::cancelDummy() {
		auto* currentState = Scipp::globalGame->stateManager.currentState;
		currentState->softDeleteEntity("dummy");

		m_dummy = nullptr;
		m_hasDummy = false;
	}

	void PlayerEntity::handleDash(const Scriptable::EventData* data) {
		if(m_isDashing) {
			if(m_dashDurationTrack >= m_dashDuration) {
				m_dashDurationTrack = sf::seconds(0);
				m_isDashing = false;
			}
			else {
				m_dashDurationTrack += data->deltaTime;	
			}
		}

		m_dashCooldownTrack += data->deltaTime;
	}

	void PlayerEntity::dash() {
		if(m_dashCooldownTrack >= m_dashCooldown) {
			m_isDashing = true;
			m_dashCooldownTrack = sf::seconds(0);
		}
	}

	void PlayerEntity::handleItems(const Scriptable::EventData* data) {
		auto items = data->currentState->getEntitiesFromGroup("items");

		auto* player_rc = getComponent<Scriptable::Components::RenderComponent>();

		for(auto const& item : items) {
			auto* item_rc = item->getComponent<Scriptable::Components::RenderComponent>();
			
			if(item_rc->isColliding(player_rc)) {
				data->currentState->playSound("pop");

				m_inventory[((Scriptable::Entities::ItemEntity*)item)->itemType]++;

				data->currentState->softDeleteEntity(item->getName());
			}
		}
	}
}