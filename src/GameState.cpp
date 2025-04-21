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
#include <Scriptable/UI/UI.hpp>

#include <SFML/Graphics.hpp>

#include <iostream>
#include <cstdlib>

const sf::Time GameState::WAVE_INTERVAL 		= sf::seconds(15);
const sf::Time GameState::WAVE_SPAWN_INTERVAL	= sf::milliseconds(300);
const int GameState::WAVE_SPAWN_AREA_OFFSET 	= 2000;
const int GameState::WAVE_SPAWN_AREA_MAX_OFFSET = 500;
const int GameState::WAVE_STARTING_ENEMY_COUNT 	= 2;
const int GameState::WAVE_ENEMY_COUNT_INCREMENT = 2;


void GameState::handleWaves(const Scriptable::EventData* data) {

	if(m_waveCooldown <= sf::seconds(0) && !m_isSpawningEnemies) {
		m_isSpawningEnemies = true;
		m_enemiesLeftToSpawn = m_waveCount * WAVE_ENEMY_COUNT_INCREMENT + WAVE_STARTING_ENEMY_COUNT;
		m_waveCooldown = WAVE_INTERVAL;
		++m_waveCount;
	}
	else if(m_isSpawningEnemies) {

		if(;!m_bossSpawned && (m_waveCount % 3 == 0)) {
			//spawnBoss(data);
			m_bossSpawned = true;
		}

		if(m_spawnEnemyCooldown <= sf::seconds(0)) {
			//spawnEnemy(data);
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

	int randomX = rand() % WAVE_SPAWN_AREA_MAX_OFFSET - (WAVE_SPAWN_AREA_MAX_OFFSET / 2);
	int randomY = rand() % WAVE_SPAWN_AREA_MAX_OFFSET - (WAVE_SPAWN_AREA_MAX_OFFSET / 2);

	static uint32_t enemy_ID = 0;

	std::string enemyName = "enemy" + std::to_string(enemy_ID);

	if(randomType == 1) {
		data->currentState->addEntity<EnemyEntity>(enemyName, sf::Vector2f(randomX, WAVE_SPAWN_AREA_OFFSET + randomY), EnemyEntity::Type::tank);
		data->currentState->addEntity<HealthbarEntity>("healthbar_enemy" + enemyName, "healthbar_enemy" + enemyName, data->currentState->getEntity("enemy" + std::to_string(enemy_ID)));
	}
	else if(randomType == 2) {
		data->currentState->addEntity<EnemyEntity>(enemyName, sf::Vector2f(randomX, WAVE_SPAWN_AREA_OFFSET + randomY), EnemyEntity::Type::speedy);
		data->currentState->addEntity<HealthbarEntity>("healthbar_enemy" + enemyName, "healthbar_enemy" + enemyName, data->currentState->getEntity("enemy" + std::to_string(enemy_ID)));
	}
	else {
		data->currentState->addEntity<EnemyEntity>(enemyName, sf::Vector2f(randomX, WAVE_SPAWN_AREA_OFFSET + randomY), EnemyEntity::Type::normal);
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
	
	data->currentState->addEntity<EnemyEntity>(enemyName, sf::Vector2f(randomX, WAVE_SPAWN_AREA_OFFSET + randomY), EnemyEntity::Type::boss);
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
	if(m_scheduledStateChange) {
		Scipp::globalGame->stateManager.changeState(m_scheduledState);
		Scipp::globalGame->stateManager.currentState->init();
	}
}

void GameState::onRender(const Scriptable::EventData* data) {
	cameraFollow();
	handleCameraShake(data->deltaTime);
	//handleWaves(data);
}

GameState::GameState()
{
	initCamera();
}

struct test_iobj : public Scriptable::UI::Rect{
	test_iobj() : Rect(sf::FloatRect({0.5,0.5}, {0.8, 0.8})){
		this->m_RenderComponent->setColor(sf::Color::Red);
	}
};


struct test_uiobj : public Scriptable::UI::Rect{
	test_uiobj() : Rect(sf::FloatRect({0,0}, {0.1, 0.1})){
		this->setAttachmentTarget("textobj", {Scriptable::UI::Object::AttachmentPoint::TOPRIGHT, {}});
		this->setLayer(1);
	}
};


void GameState::init()
{	
	Scriptable::UI::TextObject::loadFont(Util::getPathToResource("FreeMono.otf"), "font");
	//Scipp::globalGame->stateManager.currentState->addUIObject<test_iobj>("textobj");
	//Scipp::globalGame->stateManager.currentState->addUIObject<test_uiobj>("textobj2");

	addSound("pop.mp3", "pop");

	Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::PlayerEntity>("player");
	Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::HealthbarEntity>("healthbar_player", "healthbar_player", Scipp::globalGame->stateManager.currentState->getEntity("player"));

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

