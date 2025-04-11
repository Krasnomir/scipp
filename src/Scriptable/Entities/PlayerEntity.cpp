#include <Scriptable/Entities/PlayerEntity.hpp>

#include <Game.hpp>

#include <Scriptable/Components/RenderComponent.hpp>
#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Scriptable/Components/HealthComponent.hpp>
#include <Scriptable/Entities/TurretEntity.hpp>
#include <Scriptable/Entities/BulletEntity.hpp>
#include <Scriptable/Entities/HealthbarEntity.hpp>
#include <Scriptable/Entities/EnemyEntity.hpp>

namespace Scriptable::Entities {
    PlayerEntity::PlayerEntity() {
        zindex = -1;

		addComponent<Scriptable::Components::RenderComponent>(vertices);
		addComponent<Scriptable::Components::PhysicsComponent>(getComponent<Scriptable::Components::RenderComponent>());
		addComponent<Scriptable::Components::HealthComponent>(health, health, regenPerSecond, regenDelaySeconds);

		getComponent<Scriptable::Components::RenderComponent>()->setOrigin(getComponent<Scriptable::Components::RenderComponent>()->center());
		getComponent<Scriptable::Components::RenderComponent>()->addCostume("test", "res/test.png", sf::IntRect({0,0, 398, 273}));
		getComponent<Scriptable::Components::RenderComponent>()->loadCostume("test");

		Scipp::globalGame->stateManager.currentState->addEntityToGroup(this, "friendly");
    }

	void PlayerEntity::beforeRender(const Scriptable::EventData* data) {

		handleDash(data);

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
		if(data->sfmlEvent.mouseButton.button == sf::Mouse::Button::Left) {
			static uint32_t proj_ID = 0;

			auto* rc = getComponent<Scriptable::Components::RenderComponent>();
			sf::Vector2f bulletStartPosition = Util::movePoint(rc->getPosition(), bulletDistance, rc->getRotation());

			Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::BulletEntity>(std::to_string(proj_ID), rc->getRotation(), bulletStartPosition);

			proj_ID++;
		}
	}

	void PlayerEntity::onKeyPressed(const Scriptable::EventData* data) {
		if(data->sfmlEvent.key.scancode == sf::Keyboard::Scancode::Z) {
			auto* currentState = Scipp::globalGame->stateManager.currentState;
			auto* player = currentState->getEntity("test1");

			auto* closest = currentState->findClosestEntityFromGroup(player, "enemies");

			if(closest != nullptr) {
				currentState->removeEntityFromGroup(closest, "enemies");
				currentState->softDeleteEntity(closest->getName());
			}
		}
		else if(data->sfmlEvent.key.scancode == sf::Keyboard::Scancode::E) {
			static uint32_t enemy_ID = 0;

			auto* rc = getComponent<Scriptable::Components::RenderComponent>();
			sf::Vector2f enemyStartPosition = Util::movePoint(rc->getPosition(), 500, rc->getRotation());

			Scipp::globalGame->stateManager.currentState->addEntity<EnemyEntity>("enemy" + std::to_string(enemy_ID), enemyStartPosition);
			Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::HealthbarEntity>("healthbar_enemy" + std::to_string(enemy_ID), "healthbar_enemy" + std::to_string(enemy_ID), Scipp::globalGame->stateManager.currentState->getEntity("enemy" + std::to_string(enemy_ID)));
			enemy_ID++;
		}
		else if(data->sfmlEvent.key.scancode == sf::Keyboard::Scancode::Q) {
			static uint32_t turret_ID = 0;

			auto* rc = getComponent<Scriptable::Components::RenderComponent>();
			sf::Vector2f turretStartPosition = Util::movePoint(rc->getPosition(), 300, rc->getRotation());

			Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::TurretEntity>("turret" + std::to_string(turret_ID), turretStartPosition);
			Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::HealthbarEntity>("healthbar_turret" + std::to_string(turret_ID), "healthbar_turret" + std::to_string(turret_ID), Scipp::globalGame->stateManager.currentState->getEntity("turret" + std::to_string(turret_ID)));
			turret_ID++;
		}
		else if(data->sfmlEvent.key.scancode == sf::Keyboard::Scancode::R) {
			//auto* gamestate = (GameState*) Scipp::globalGame->stateManager.currentState;
			//gamestate->shakeCamera(10, 15);
		}
		else if(data->sfmlEvent.key.scancode == sf::Keyboard::Scancode::F) {
			dash();
		}
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
}