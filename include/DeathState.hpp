#pragma once

#include <Scriptable/State.hpp>
#include <Scriptable/Entity.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <vector>

class DeathState : public Scriptable::State
{
	

public:
	DeathState();

	void init();

	void onWindowClosed(const Scriptable::EventData* data);

	void onWindowResized(const Scriptable::EventData* data);
};