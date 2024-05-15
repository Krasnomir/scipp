#pragma once

#include "State.h"
#include "Entity.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include <vector>

class InitState : public State
{
	void init();
	void update(sf::Time elapsed);
public:
	void addTriangle(sf::Vector2f pos);

	InitState();
};