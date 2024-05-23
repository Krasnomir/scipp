#include <Game.h>
#include <StateManager.h>
#include <State.h>
#include <InitState.h>
#include <Scriptable/Entity.hpp>

#include <SFML/Graphics.hpp>

#include <iostream>

namespace Scipp {
	Game* globalGame = nullptr;
	StateManager* globalManager = nullptr;
}

void Game::handleEvents(sf::Event event)
{
	
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

	
	this->window->display();
}


struct DebugComponent : public Scriptable::Component
{
	void beforeRender(const EventData* data) 
	{
		printf("before render component called\n");

	}

    void afterRender(const EventData* data)
	{
		printf("after render component called\n");
	} 
};

struct DebugEntity : public Scriptable::Entity
{
	void beforeRender(const EventData* data) 
	{
		printf("before render entity called\n");

	}

    void afterRender(const EventData* data)
	{
		printf("after render entity called\n");
	} 
};

void Game::run() 
{
	{
		DebugEntity TestEntity;
		TestEntity.addComponent<DebugComponent>();

		TestEntity.evokeAll("beforeRender", nullptr);
		TestEntity.evokeAll("afterRender", nullptr);
		
	}

	
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