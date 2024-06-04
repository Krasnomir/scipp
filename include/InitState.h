#pragma once

#include <Scriptable/State.hpp>
#include <Scriptable/Entity.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <vector>

class InitState : public Scriptable::State
{
	public:
		InitState();

		void onRender(const Scriptable::EventData* data);

		void onWindowClosed(const Scriptable::EventData* data);

};