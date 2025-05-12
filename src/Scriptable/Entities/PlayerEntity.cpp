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
#include <Scriptable/Entities/MudTrapEntity.hpp>
#include <Scriptable/Entities/SpikeTrapEntity.hpp>
#include <Misc/Explosion.hpp>
#include <Scriptable/UI/Inventory.hpp>

#include <iostream>

namespace Scriptable::Entities {
	sf::Color const PlayerEntity::DUMMY_COLOR_ALLOWED 	= sf::Color(80, 210, 80, 100);
	sf::Color const PlayerEntity::DUMMY_COLOR_FORBIDDEN = sf::Color(220, 70, 70, 100);
	short const PlayerEntity::DUMMY_COLOR_ALPHA 		= 150;
	short const PlayerEntity::DUMMY_ZINDEX				= 10;

	sf::Time const PlayerEntity::GUN_COOLDOWN = sf::seconds(0.5);
	int const PlayerEntity::GUN_DAMAGE = 100;

	bool PlayerEntity::pay(std::unordered_map<ItemEntity::Item, int> requiredItems) {
		auto inv_clone = m_inventory;

		for(auto& requiredItem : requiredItems) {
			if(inv_clone[requiredItem.first] >= requiredItem.second) {
				inv_clone[requiredItem.first] -= requiredItem.second;
			}
			else {
				return false;
			}
		}

		m_inventory = inv_clone;
		return true;
	}

    PlayerEntity::PlayerEntity() {
        zindex = 1;

		addComponent<Scriptable::Components::RenderComponent>(vertices);
		addComponent<Scriptable::Components::PhysicsComponent>(getComponent<Scriptable::Components::RenderComponent>());
		addComponent<Scriptable::Components::HealthComponent>(health, health, regenPerSecond, regenDelaySeconds);
		
		getComponent<Scriptable::Components::RenderComponent>()->setOrigin(getComponent<Scriptable::Components::RenderComponent>()->center());
		getComponent<Scriptable::Components::RenderComponent>()->addCostume("test", Util::getPathToResource("test.png"), sf::IntRect({0,0, 398, 273}));
		getComponent<Scriptable::Components::RenderComponent>()->loadCostume("test");
		getComponent<Scriptable::Components::RenderComponent>()->setColor(sf::Color(62, 75, 76));

		getComponent<Scriptable::Components::HealthComponent>()->setOnDeathCallback(playerDeathCallback);

		getComponent<Scriptable::Components::PhysicsComponent>()->set_collidable(true);

		Scipp::globalGame->stateManager.currentState->addEntityToGroup(this, "friendly");
    }

	void PlayerEntity::beforeRender(const Scriptable::EventData* data) {

		handleGun(data);

		handleDash(data);
		handleDummy();

		handleItems(data);

		handleMovement();
	}

	void PlayerEntity::onMouseMoved(const Scriptable::EventData* data) {
		auto mousePos = Scipp::globalGame->stateManager.currentState->M_camera.getMousePositionRelativeToCamera(true);
		auto* renderComponent = getComponent<Scriptable::Components::RenderComponent>();


		renderComponent->setRotation(Util::getAngleBetweenPoints(renderComponent->getPosition(), mousePos));
	}

	void PlayerEntity::onMouseButtonPressed(const Scriptable::EventData* data) {
		if(data->sfmlEvent.mouseButton.button == sf::Mouse::Button::Right) {
			requestPlacement(data);
		}
		if(data->sfmlEvent.mouseButton.button == sf::Mouse::Button::Left) {
			requestFiring();
		}
	}

	void PlayerEntity::onKeyPressed(const Scriptable::EventData* data) {
		if(data->sfmlEvent.key.scancode == sf::Keyboard::Scancode::E) {
			requestDummy(m_placementPanel[m_placementPanelIndex]);
		}
		// switch to the next dummy type
		else if(data->sfmlEvent.key.scancode == sf::Keyboard::Scancode::R) {
			if(m_hasDummy) {
				m_placementPanelIndex = (m_placementPanelIndex + 1) % m_placementPanel.size();
				requestDummy(m_placementPanel[m_placementPanelIndex]);
			}
		}
		else if(data->sfmlEvent.key.scancode == sf::Keyboard::Scancode::Q) {
			cancelDummy();
		}
		/*
		else if(data->sfmlEvent.key.scancode == sf::Keyboard::Scancode::G) {
			static uint32_t enemy_ID = 0;

			auto* rc = getComponent<Scriptable::Components::RenderComponent>();
			sf::Vector2f enemyStartPosition = Util::movePoint(rc->getPosition(), 500, rc->getRotation());

			Scipp::globalGame->stateManager.currentState->addEntity<EnemyEntity>("enemy" + std::to_string(enemy_ID), enemyStartPosition, EnemyEntity::Type::normal, 1);
			Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::HealthbarEntity>("healthbar_enemy" + std::to_string(enemy_ID), "healthbar_enemy" + std::to_string(enemy_ID), Scipp::globalGame->stateManager.currentState->getEntity("enemy" + std::to_string(enemy_ID)));
			enemy_ID++;
		}
		*/
		else if(data->sfmlEvent.key.scancode == sf::Keyboard::Scancode::F) {
			dash();
		}
		/*
		else if(data->sfmlEvent.key.scancode == sf::Keyboard::Scancode::I){
			// INVENTORY
			if(!Scipp::globalGame->stateManager.currentState->hasUIObject("inventory_frame")){
				Scipp::globalGame->stateManager.currentState->addUIObject<Scriptable::UI::InventoryFrame>("inventory_frame");
			}
			auto* invObj = dynamic_cast<Scriptable::UI::InventoryFrame*>(Scipp::globalGame->stateManager.currentState->getUIObject("inventory_frame"));
			if(invObj->getVisible()) invObj->hide();
			else invObj->show();
			


		}
		*/
	}

	void PlayerEntity::handleGun(const Scriptable::EventData* data) {
		if(m_bulletCooldown > sf::seconds(0)) {
			m_bulletCooldown -= data->deltaTime;
		}
	}

	void PlayerEntity::requestFiring() {
		if(m_bulletCooldown <= sf::seconds(0)) {
			static uint32_t proj_ID = 0;

			auto* rc = getComponent<Scriptable::Components::RenderComponent>();
			sf::Vector2f bulletStartPosition = Util::movePoint(rc->getPosition(), bulletDistance, rc->getRotation());

			Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::BulletEntity>("bullet_" + std::to_string(proj_ID), rc->getRotation(), bulletStartPosition, GUN_DAMAGE);

			proj_ID++;

			m_bulletCooldown = GUN_COOLDOWN;

			explosion(bulletStartPosition, 3, 50, 100, 300, sf::Color(200,200,0,20));
		}
	}

	void PlayerEntity::requestDummy(m_dummy_type type) {
		if(m_hasDummy) {
			auto* rc = m_dummy->getComponent<Scriptable::Components::RenderComponent>();

			rc->setVertices(m_dummy_vertices[type]);
			rc->setOrigin(rc->center());
			m_currentDummyType = type;
		}
		else {
			auto* currentState = Scipp::globalGame->stateManager.currentState;

			currentState->addEntity<Scriptable::Entities::SimpleEntity>("dummy", m_dummy_vertices[type]);
			m_dummy = currentState->getEntity("dummy");

			m_dummy->zindex = DUMMY_ZINDEX;

			auto* rc = m_dummy->getComponent<Scriptable::Components::RenderComponent>();

			rc->setColor(DUMMY_COLOR_FORBIDDEN);

			m_hasDummy = true;
			m_currentDummyType = type;
		}
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
		if(m_hasDummy) {
			auto* currentState = Scipp::globalGame->stateManager.currentState;
			currentState->softDeleteEntity("dummy");

			m_dummy = nullptr;
			m_hasDummy = false;
		}
	}

	void PlayerEntity::requestPlacement(const Scriptable::EventData* data) {

		if(!(m_hasDummy && m_dummyAllowed)) return;

		auto* rc = m_dummy->getComponent<Scriptable::Components::RenderComponent>();

		if(m_currentDummyType == m_dummy_type::turret) {
			if(pay(m_dummy_recipes[m_currentDummyType])) {
				static uint32_t turret_ID = 0;
	
				data->currentState->addEntity<Scriptable::Entities::TurretEntity>("turret" + std::to_string(turret_ID), rc->getPosition());
				data->currentState->addEntity<Scriptable::Entities::HealthbarEntity>("healthbar_turret" + std::to_string(turret_ID), "healthbar_turret" + std::to_string(turret_ID), Scipp::globalGame->stateManager.currentState->getEntity("turret" + std::to_string(turret_ID)));
				turret_ID++;
	
				m_inventory[ItemEntity::Item::steel]--;
			}
		}
		else if(m_currentDummyType == m_dummy_type::mud_trap) {
			static uint32_t mudTrap_ID = 0;

			data->currentState->addEntity<Scriptable::Entities::MudTrapEntity>("mud_trap_"+mudTrap_ID, rc->getPosition());

			++mudTrap_ID;
		}
		else if(m_currentDummyType == m_dummy_type::spike_trap) {
			static uint32_t spikeTrap_ID = 0;

			data->currentState->addEntity<Scriptable::Entities::SpikeTrapEntity>("spike_trap_"+spikeTrap_ID, rc->getPosition());

			++spikeTrap_ID;
		}

		cancelDummy();
	}

	void PlayerEntity::handleDash(const Scriptable::EventData* data) {
		if(m_isDashing) {
			if(m_dashDurationTrack >= m_dashDuration) {
				m_dashDurationTrack = sf::seconds(0);
				m_isDashing = false;
				m_dash_vec = sf::Vector2f(0,0);
			}
			else {
				m_dashDurationTrack += data->deltaTime;	
			}
		}

		m_dashCooldownTrack += data->deltaTime;
	}

	void PlayerEntity::dash() {
		if(m_dashCooldownTrack >= m_dashCooldown) {
			float direction = getComponent<Scriptable::Components::RenderComponent>()->getRotation();

			m_isDashing = true;
			m_dashCooldownTrack = sf::seconds(0);
			m_dash_vec = Util::vec_from_mag_and_dir(m_dashSpeed, direction);
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

	void PlayerEntity::handleMovement() {
		auto* rc = this->getComponent<Scriptable::Components::RenderComponent>();

		sf::Vector2f direction = {0,0};

		bool w_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);
		bool a_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
		bool s_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
		bool d_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

		if(w_pressed)
			direction.y -= 1.f;
		if(s_pressed)
			direction.y += 1.f;
		if(a_pressed)
			direction.x -= 1.f;
		if(d_pressed)
			direction.x += 1.f;
		
		auto* pc = getComponent<Scriptable::Components::PhysicsComponent>();
		direction = Util::vec_normalize(direction);
		auto dash_value = m_dash_vec * (m_dashDurationTrack / m_dashDuration);
		pc->velocity = sf::Vector2f(direction.x * PLAYER_SPEED + dash_value.x, direction.y * PLAYER_SPEED + dash_value.y);
	}
}