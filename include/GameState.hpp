#pragma once

#include <Scriptable/State.hpp>
#include <Scriptable/Entity.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <vector>

class GameState : public Scriptable::State
{
public:
	GameState();

	void init();

	void onWindowClosed(const Scriptable::EventData* data);
};