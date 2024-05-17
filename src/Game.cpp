#include "Game.h"
#include "StateManager.h"
#include "State.h"
#include "InitState.h"
#include "Entity.h"

#include <SFML/Graphics.hpp>

#include <iostream>

namespace Scipp {
	Game* globalGame = nullptr;
	StateManager* globalManager = nullptr;
}

void Game::handleEvents(sf::Event event)
{
	if (event.type == sf::Event::Closed)
	{
		this->window->close();
	}
}

void Game::pollEvents()
{
	sf::Event event;
	while (this->window->pollEvent(event)) handleEvents(event);
}

void Game::update(sf::Time elapsed)
{
	this->stateManager->currentState->update(elapsed);
	// std::cout << elapsed.asMilliseconds() << "\n";
}

void Game::render()
{
	this->window->clear(sf::Color::Blue);

	for (Entity e : this->stateManager->currentState->components)
	{
		this->window->draw(e);
	}

	this->window->display();
}

void Game::run() 
{
	while (this->window->isOpen()) 
	{
		sf::Time elapsed = this->clock.restart();

		this->pollEvents();
		this->update(elapsed);
		this->render();
	}
}

void Game::initWindow() 
{
	this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "scipp");
	this->window->setFramerateLimit(60);
}


void Game::initStates()
{
	this->stateManager = new StateManager();
	Scipp::globalManager = this->stateManager;


	this->stateManager->changeState(new InitState());
}

void Game::init() 
{
	Scipp::globalGame = this;

	this->initWindow();
	this->initStates();
}

Game::Game() 
{
	this->init();
	this->run();
}