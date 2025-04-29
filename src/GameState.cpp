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
#include <Scriptable/Entities/HealthbarEntity.hpp>
#include <Scriptable/Entities/PlayerEntity.hpp>
#include <Scriptable/Entities/EnemyEntity.hpp>
#include <Scriptable/Entities/SimpleEntity.hpp>
#include <Scriptable/UI/UI.hpp>
#include <SoundManager.hpp>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <cstdlib>

const sf::Time GameState::WAVE_INTERVAL 				= sf::seconds(10);
const sf::Time GameState::WAVE_SPAWN_INTERVAL			= sf::milliseconds(500);
const int GameState::WAVE_SPAWN_AREA_OFFSET 			= 1000;
const int GameState::WAVE_SPAWN_AREA_MAX_OFFSET 		= 500;
const int GameState::WAVE_STARTING_ENEMY_COUNT 			= 1;
const int GameState::WAVE_ENEMY_COUNT_INCREMENT 		= 1;
const float GameState::WAVE_HEALTH_MULTIPLIER_INCREMENT = 0.5;


void GameState::handleWaves(const Scriptable::EventData* data) {

	if(m_waveCooldown <= sf::seconds(0) && !m_isSpawningEnemies) {
		++m_waveCount;
		m_isSpawningEnemies = true;
		m_enemiesLeftToSpawn = WAVE_STARTING_ENEMY_COUNT + m_waveCount * WAVE_ENEMY_COUNT_INCREMENT - WAVE_ENEMY_COUNT_INCREMENT;
		m_waveCooldown = WAVE_INTERVAL;
	}
	else if(m_isSpawningEnemies) {

		if(!m_bossSpawned && (m_waveCount % 3 == 0)) {
			spawnBoss(data);
			m_bossSpawned = true;
		}

		if(m_spawnEnemyCooldown <= sf::seconds(0) && m_enemiesLeftToSpawn > 0) {
			spawnEnemy(data);
			m_spawnEnemyCooldown = WAVE_SPAWN_INTERVAL;
			--m_enemiesLeftToSpawn;
		}
		else {
			m_spawnEnemyCooldown -= data->deltaTime;
		}

		if(m_enemiesLeftToSpawn == 0) {
			m_isSpawningEnemies = false;
			m_bossSpawned = false;
		}
	}
	else {
		m_waveCooldown -= data->deltaTime;
	}
}

void GameState::spawnEnemy(const Scriptable::EventData* data) {

	using namespace Scriptable::Entities;

	int randomType = rand() % 8 + 1;

	short random_angle = rand() % 360;

	sf::Vector2f spawn_position = Util::movePoint(sf::Vector2f(0,0), WAVE_SPAWN_AREA_OFFSET, random_angle);

	static uint32_t enemy_ID = 0;

	std::string enemyName = "enemy" + std::to_string(enemy_ID);

	float healthMultiplier = 1 + (m_waveCount * WAVE_HEALTH_MULTIPLIER_INCREMENT) - WAVE_HEALTH_MULTIPLIER_INCREMENT;

	if(randomType == 1) {
		data->currentState->addEntity<EnemyEntity>(enemyName, spawn_position, EnemyEntity::Type::tank, healthMultiplier);
		data->currentState->addEntity<HealthbarEntity>("healthbar_enemy" + enemyName, "healthbar_enemy" + enemyName, data->currentState->getEntity("enemy" + std::to_string(enemy_ID)));
	}
	else if(randomType == 2) {
		data->currentState->addEntity<EnemyEntity>(enemyName, spawn_position, EnemyEntity::Type::speedy, healthMultiplier);
		data->currentState->addEntity<HealthbarEntity>("healthbar_enemy" + enemyName, "healthbar_enemy" + enemyName, data->currentState->getEntity("enemy" + std::to_string(enemy_ID)));
	}
	else {
		data->currentState->addEntity<EnemyEntity>(enemyName, spawn_position, EnemyEntity::Type::normal, healthMultiplier);
		data->currentState->addEntity<HealthbarEntity>("healthbar_enemy" + enemyName, "healthbar_enemy" + enemyName, data->currentState->getEntity("enemy" + std::to_string(enemy_ID)));
	}

	++enemy_ID;
}

void GameState::spawnBoss(const Scriptable::EventData* data) {
	
	using namespace Scriptable::Entities;

	int randomX = rand() % WAVE_SPAWN_AREA_MAX_OFFSET - (WAVE_SPAWN_AREA_MAX_OFFSET / 2);
	int randomY = rand() % WAVE_SPAWN_AREA_MAX_OFFSET - (WAVE_SPAWN_AREA_MAX_OFFSET / 2);

	static uint32_t boss_ID = 0;

	std::string enemyName = "boss" + std::to_string(boss_ID);
	
	data->currentState->addEntity<EnemyEntity>(enemyName, sf::Vector2f(randomX, WAVE_SPAWN_AREA_OFFSET + randomY), EnemyEntity::Type::boss, 1);
	data->currentState->addEntity<HealthbarEntity>("healthbar_boss" + enemyName, "healthbar_boss" + enemyName, data->currentState->getEntity("boss" + std::to_string(boss_ID)));

	++boss_ID;
}

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
	sf::Vector2f playerPosition = getEntity("player")->getComponent<Scriptable::Components::RenderComponent>()->getPosition();
	// Camera position will be slightly shifted to where the player is looking (where the mouse cursor is) it also depends on how far from center it is 
	float shiftIntensity = 20; // The smaller the number the bigger the intensity is going to be
	float xShift = M_camera.getMousePositionRelativeToCamera().x / shiftIntensity;
	float yShift = M_camera.getMousePositionRelativeToCamera().y / shiftIntensity;

	M_camera.setPosition(sf::Vector2f(playerPosition.x + xShift, playerPosition.y + yShift));
	M_camera.apply();
}

void GameState::beforeRender(const Scriptable::EventData* data) {
	handleCameraShake(data->deltaTime);
	//handleWaves(data);
}

void GameState::onRender(const Scriptable::EventData* data) {
	cameraFollow();
}

GameState::GameState()
{
	
}



void GameState::init()
{	
	initCamera(sf::Vector2f(Scipp::globalGame->window->getSize().x, Scipp::globalGame->window->getSize().y));
	
	Scriptable::UI::TextObject::loadFont("RobotoMono.ttf", "font");

	Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::PlayerEntity>("player");
	auto* entity = Scipp::globalGame->stateManager.currentState->getEntity("player");
	
	Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::HealthbarEntity>("healthbar_player", "healthbar_player", Scipp::globalGame->stateManager.currentState->getEntity("player"));

	initMap();

}

void GameState::initMap() {
	std::vector<sf::Vector2f> vertices = {
		{0, 0}, {1600, 0}, {1600, 30}, {0, 0}, {1600, 30}, {0, 30}
	};
	
	// Top wall (above center by 500px)
	Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::SimpleEntity>("wall_top", vertices);
	auto* wallTop = Scipp::globalGame->stateManager.currentState->getEntity("wall_top");
	auto* rcTop = wallTop->getComponent<Scriptable::Components::RenderComponent>();
	rcTop->setColor(sf::Color(85, 85, 55));
	rcTop->setPosition(sf::Vector2f(0, -800));
	wallTop->addComponent<Scriptable::Components::PhysicsComponent>(rcTop);
	wallTop->getComponent<Scriptable::Components::PhysicsComponent>()->set_collidable(true);
	
	// Bottom wall (below center by 500px)
	Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::SimpleEntity>("wall_bottom", vertices);
	auto* wallBottom = Scipp::globalGame->stateManager.currentState->getEntity("wall_bottom");
	auto* rcBottom = wallBottom->getComponent<Scriptable::Components::RenderComponent>();
	rcBottom->setColor(sf::Color(85, 85, 55));
	rcBottom->setPosition(sf::Vector2f(0, 800));
	wallBottom->addComponent<Scriptable::Components::PhysicsComponent>(rcBottom);
	wallBottom->getComponent<Scriptable::Components::PhysicsComponent>()->set_collidable(true);
	
	// Left wall (left of center by 500px, vertical wall)
	Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::SimpleEntity>("wall_left", vertices);
	auto* wallLeft = Scipp::globalGame->stateManager.currentState->getEntity("wall_left");
	auto* rcLeft = wallLeft->getComponent<Scriptable::Components::RenderComponent>();
	rcLeft->setColor(sf::Color(85, 85, 55));
	rcLeft->setPosition(sf::Vector2f(-800, 0));
	rcLeft->setRotation(90.f);
	wallLeft->addComponent<Scriptable::Components::PhysicsComponent>(rcLeft);
	wallLeft->getComponent<Scriptable::Components::PhysicsComponent>()->set_collidable(true);
	
	// Right wall (right of center by 500px, vertical wall)
	Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::SimpleEntity>("wall_right", vertices);
	auto* wallRight = Scipp::globalGame->stateManager.currentState->getEntity("wall_right");
	auto* rcRight = wallRight->getComponent<Scriptable::Components::RenderComponent>();
	rcRight->setColor(sf::Color(85, 85, 55));
	rcRight->setPosition(sf::Vector2f(800, 0));
	rcRight->setRotation(90.f);
	wallRight->addComponent<Scriptable::Components::PhysicsComponent>(rcRight);
	wallRight->getComponent<Scriptable::Components::PhysicsComponent>()->set_collidable(true);

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

