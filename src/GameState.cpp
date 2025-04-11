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
	Scipp::globalGame->stateManager.currentState->addUIObject<test_uiobj>("texttest");	
	Scipp::globalGame->stateManager.currentState->addEntity<Scriptable::Entities::PlayerEntity>("test1");
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

