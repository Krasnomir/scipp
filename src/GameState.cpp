#include <Game.hpp>
#include <StateManager.hpp>
#include <Camera.hpp>
#include <Util.hpp>
#include <GameState.hpp>
#include <Scriptable/State.hpp>
#include <Scriptable/Entity.hpp>
#include <Scriptable/Components/RenderComponent.hpp>
#include <Scriptable/Components/LifetimeComponent.hpp>
#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Scriptable/Components/HealthComponent.hpp>
#include <Scriptable/Components/EnemyComponent.hpp>
#include <Scriptable/Entities/TurretEntity.hpp>
#include <Scriptable/Entities/BulletEntity.hpp>
#include <Scriptable/UI/UI.hpp>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <cstdlib>


struct EnemyEntity : public Scriptable::Entity {

	float health = 100;
	std::string groupName = "hostile";

	std::vector<sf::Vector2f> vertices = {{0,0}, {0,50}, {50, 0}, {0, 50}, {50,50}, {50,0}};

	static void deleteEnemyCallback(Scriptable::Components::HealthComponent* c) {
		Scipp::globalGame->stateManager.currentState->softDeleteEntity(((Scriptable::Entity*)c->parentEntity)->getName());
	}

	EnemyEntity(sf::Vector2f pos) {
		
		addComponent<Scriptable::Components::RenderComponent>(vertices);
		addComponent<Scriptable::Components::PhysicsComponent>(getComponent<Scriptable::Components::RenderComponent>());
		addComponent<Scriptable::Components::EnemyComponent>();
		addComponent<Scriptable::Components::HealthComponent>(health);

		getComponent<Scriptable::Components::RenderComponent>()->setPosition(pos);
		getComponent<Scriptable::Components::RenderComponent>()->setOrigin(getComponent<Scriptable::Components::RenderComponent>()->center());
		getComponent<Scriptable::Components::HealthComponent>()->setOnDeathCallback(deleteEnemyCallback);

		Scipp::globalGame->stateManager.currentState->addEntityToGroup(this, groupName);

	}

	virtual ~EnemyEntity() = default;
};

struct PlayerEntity : public Scriptable::Entity {
	float health = 100;
	float regenPerSecond = 10;
	float regenDelaySeconds = 5;
	float bulletDistance = 50;

	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> vertices = {{{0,0}, {18,70}}, {{0, 100}, {18, 170}}, {{30, 0},  {48, 70}}, {{30,0}, {48, 70}}, {{30,100}, {48, 170}},{{0,100},{18, 170}}};

	virtual ~PlayerEntity() = default;

	PlayerEntity() {
		addComponent<Scriptable::Components::RenderComponent>(vertices);
		addComponent<Scriptable::Components::PhysicsComponent>(getComponent<Scriptable::Components::RenderComponent>());
		addComponent<Scriptable::Components::HealthComponent>(health, health, regenPerSecond, regenDelaySeconds);

		getComponent<Scriptable::Components::RenderComponent>()->setOrigin(getComponent<Scriptable::Components::RenderComponent>()->center());
		getComponent<Scriptable::Components::RenderComponent>()->addCostume("test", "test.png", sf::IntRect({0,0, 398, 273}));
		getComponent<Scriptable::Components::RenderComponent>()->loadCostume("test");

		Scipp::globalGame->stateManager.currentState->addEntityToGroup(this, "friendly");
	}

	void beforeRender(const Scriptable::EventData* data) {
		
		auto* rc = this->getComponent<Scriptable::Components::RenderComponent>();

		// wasd movement
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
			auto* pc = getComponent<Scriptable::Components::PhysicsComponent>();
			pc->velocity.magnitude = 5;
			pc->velocity.direction = rc->getRotation();
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
			auto* pc = getComponent<Scriptable::Components::PhysicsComponent>();
			pc->velocity.magnitude = 5;
			pc->velocity.direction = rc->getRotation() - 180;
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
			auto* pc = getComponent<Scriptable::Components::PhysicsComponent>();
			pc->velocity.magnitude = 5;
			pc->velocity.direction = rc->getRotation() - 90;
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
			auto* pc = getComponent<Scriptable::Components::PhysicsComponent>();
			pc->velocity.magnitude = 5;
			pc->velocity.direction = rc->getRotation() + 90;
		}
		else {
			getComponent<Scriptable::Components::PhysicsComponent>()->velocity.magnitude = 0;
		}
	}

	void onMouseMoved(const Scriptable::EventData* data) {
		auto mousePos = Scipp::globalGame->stateManager.currentState->M_camera.getMousePositionRelativeToCamera(true);
		auto* renderComponent = getComponent<Scriptable::Components::RenderComponent>();

		renderComponent->setRotation(Util::getAngleBetweenPoints(renderComponent->getPosition(), mousePos));
	}

	void onMouseButtonPressed(const Scriptable::EventData* data) {
		if(data->sfmlEvent.mouseButton.button == sf::Mouse::Button::Left) {
			static uint32_t proj_ID = 0;

			auto* rc = getComponent<Scriptable::Components::RenderComponent>();
			sf::Vector2f bulletStartPosition = Util::movePoint(rc->getPosition(), bulletDistance, rc->getRotation());

			Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::BulletEntity>(std::to_string(proj_ID), rc->getRotation(), bulletStartPosition);

			proj_ID++;
		}
	}

	void onKeyPressed(const Scriptable::EventData* data) {
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
			enemy_ID++;
		}
		else if(data->sfmlEvent.key.scancode == sf::Keyboard::Scancode::Q) {
			static uint32_t turret_ID = 0;

			auto* rc = getComponent<Scriptable::Components::RenderComponent>();
			sf::Vector2f turretStartPosition = Util::movePoint(rc->getPosition(), 300, rc->getRotation());

			Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::TurretEntity>("turret" + std::to_string(turret_ID), turretStartPosition);
			turret_ID++;
		}
		else if(data->sfmlEvent.key.scancode == sf::Keyboard::Scancode::R) {
			auto* gamestate = (GameState*) Scipp::globalGame->stateManager.currentState;
			gamestate->shakeCamera(10, 15);
		}
	}

};

void GameState::onWindowClosed(const Scriptable::EventData* data)
{
	data->targetWindow->close();
}

void GameState::onWindowResized(const Scriptable::EventData* data){
	Camera newCamera((sf::Vector2f)data->targetWindow->getSize(), data->currentState->M_camera.getPosition(), sf::FloatRect({0,0,1,1}));
	M_camera = newCamera;
	M_camera.apply();
}

void GameState::cameraFollow() {
	sf::Vector2f playerPosition = getEntity("test1")->getComponent<Scriptable::Components::RenderComponent>()->getPosition();
	// Camera position will be slightly shifted to where the player is looking (where the mouse cursor is) it also depends on how far from center it is 
	float shiftIntensity = 20; // The smaller the number the bigger the intensity is going to be
	float xShift = M_camera.getMousePositionRelativeToCamera().x / shiftIntensity;
	float yShift = M_camera.getMousePositionRelativeToCamera().y / shiftIntensity;

	M_camera.setPosition(sf::Vector2f(playerPosition.x + xShift, playerPosition.y + yShift));
	M_camera.apply();
}

void GameState::onRender(const Scriptable::EventData* data) {
	cameraFollow();
	handleCameraShake(data->deltaTime);
}

GameState::GameState()
{
	initCamera();
}

struct test_uiobj : public Scriptable::UI::TextObject{
	test_uiobj() : TextObject("debug"){
		Text::setPosition({100,100});
		Text::setString("hello world");
	}

		
};



void GameState::init()
{

	printf("Font: %d\n", Scriptable::UI::TextObject::loadFont("./FreeMono.otf", "debug"));
	
	Scipp::globalGame->stateManager.currentState->addUIObject<test_uiobj>("texttest");

	
	Scipp::globalGame->stateManager.currentState->addEntity<PlayerEntity>("test1");
}

void GameState::shakeCamera(int minShake, int maxShake) {
	if(m_isGoingBack || m_isTilting) return; // prevents two shakes from happening at the same time

	int randomRotation = rand() % (maxShake-minShake) + minShake;
	if(rand() % 2 == 1) {
		randomRotation = -randomRotation;
		m_isReverse = true;
	} 
	else {
		m_isReverse = false;
	}
	
	float goal = M_camera.getRotation() + randomRotation;

	m_isTilting = true;
	m_startingRotation = M_camera.getRotation();
	m_currentRotation = M_camera.getRotation();
	m_endingRotation = goal;
}

// BEWARE! nesting hell ahead
void GameState::handleCameraShake(sf::Time deltaTime) {
	if(m_isTilting || m_isGoingBack) {
		float increment = deltaTime.asSeconds() * m_shakeIncrement;

		if(m_isTilting) {
			if(m_isReverse) {
				if(m_currentRotation - increment <= m_endingRotation) {
					m_currentRotation = m_endingRotation;
					M_camera.setRotation(Util::numToAngle(m_currentRotation));
					m_isGoingBack = true;
					m_isTilting = false;
					return;
				}
				m_currentRotation = m_currentRotation - increment;
				M_camera.setRotation(Util::numToAngle(m_currentRotation));
			}
			else {
				if(m_currentRotation + increment >= m_endingRotation) {
					m_currentRotation = m_endingRotation;
					M_camera.setRotation(Util::numToAngle(m_currentRotation));
					m_isGoingBack = true;
					m_isTilting = false;
					return;
				}
				m_currentRotation = m_currentRotation + increment;
				M_camera.setRotation(Util::numToAngle(m_currentRotation));
			}
		}
		else if(m_isGoingBack) {
			if(m_isReverse) {
				if(m_currentRotation + increment >= m_startingRotation) {
					M_camera.setRotation(Util::numToAngle(m_startingRotation));
					m_isGoingBack = false;
					return;
				}
				m_currentRotation = m_currentRotation + increment;
				M_camera.setRotation(Util::numToAngle(m_currentRotation));
			}
			else {
				if(m_currentRotation - increment <= m_startingRotation) {
					M_camera.setRotation(Util::numToAngle(m_startingRotation));
					m_isGoingBack = false;
					return;
				}
				m_currentRotation = m_currentRotation - increment;
				M_camera.setRotation(Util::numToAngle(m_currentRotation));
			}
		}
	}
}

