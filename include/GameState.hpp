#pragma once

#include <Scriptable/State.hpp>
#include <Scriptable/Entity.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <vector>

class GameState : public Scriptable::State
{
	void cameraFollow();

	// camera shake variables
	float m_shakeIncrement = 200; // degrees per seond
	float m_currentRotation; // has to be stored in seperate variable as normal rotation is in (0-360) range which fucks up everything
	float m_startingRotation;
	float m_endingRotation;
	bool m_isReverse = false;
	bool m_isTilting = false;
	bool m_isGoingBack = false;

	static const sf::Time 	WAVE_INTERVAL;
	static const int 		WAVE_SPAWN_AREA_OFFSET;

	sf::Time m_waveCooldown = sf::seconds(0);

public:
	GameState();

	void init();

	void onWindowClosed(const Scriptable::EventData* data);

	void onWindowResized(const Scriptable::EventData* data);

	void onRender(const Scriptable::EventData* data);

	void shakeCamera(int minShake, int maxShake);
	void handleCameraShake(sf::Time deltaTime);
	void handleDash(sf::Time deltaTime);

	void handleWaves(const Scriptable::EventData* data);
};