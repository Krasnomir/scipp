#include "Game.h"
#include "StateManager.h"
#include "State.h"
#include "TestState.h"

#include <SFML/Graphics.hpp>

#include <iostream>

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
	this->stateManager->currentState->update();
	// std::cout << elapsed.asMilliseconds() << "\n";
}

void Game::render()
{
	this->window->clear(sf::Color::Blue);

	for (sf::VertexArray va : this->stateManager->currentState->components)
	{
		this->window->draw(va);
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
	this->window->setFramerateLimit(30);
}

void Game::initStates()
{
	this->stateManager = new StateManager(this);
	this->stateManager->changeState(new TestState(this->stateManager));
}

void Game::init() 
{
	this->initStates();
	this->initWindow();
}

Game::Game() 
{
	this->init();
	this->run();
}