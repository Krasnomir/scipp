#pragma once

#include <State.h>
#include <Scriptable/Entity.hpp>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <vector>

class InitState : public State
{
	void init();
	void update(sf::Time elapsed);
public:

	InitState();
};