#include <Game.hpp>
#include <Util.hpp>
#include <StateManager.hpp>
#include <Scriptable/State.hpp>
#include <InitState.hpp>
#include <Scriptable/Entity.hpp>
#include <Scriptable/EventObject.hpp>
#include <Scriptable/Components/RenderComponent.hpp>

#include <SFML/Graphics.hpp>

#include <math.h>
#include <string>
#include <iostream>

namespace Scipp {
	Game* globalGame = nullptr;
}

void Game::handleEvent(sf::Event event)
{
	M_eventData.sfmlEvent = event;

	switch(event.type){
		case sf::Event::Closed:
		{
			stateManager.currentState->evokeAll("onWindowClosed", &M_eventData);
			break;
		}

		case sf::Event::MouseMoved:
		{
			stateManager.currentState->evokeAll("onMouseMoved", &M_eventData);
			break;
		}

		case sf::Event::KeyPressed:
		{
			stateManager.currentState->evokeAll("onKeyPressed", &M_eventData);
			break;
		}

		case sf::Event::KeyReleased:
		{
			stateManager.currentState->evokeAll("onKeyReleased", &M_eventData);
			break;
		}

		case sf::Event::MouseButtonPressed:
		{
			stateManager.currentState->evokeAll("onMouseButtonPressed", &M_eventData);
			break;
		}

		case sf::Event::MouseButtonReleased:
		{
			stateManager.currentState->evokeAll("onMouseButtonReleased", &M_eventData);
			break;
		}

		default:
		{
			//unhandled events
			break;
		}
	}
}

void Game::pollEvents()
{
	sf::Event event;
	while (this->window->pollEvent(event)) handleEvent(event);
}

void Game::run() 
{
	while (this->window->isOpen()) 
	{
		deltaTime = M_clock.restart();

		M_eventData.currentState = stateManager.currentState;
		M_eventData.targetWindow = window;
		M_eventData.deltaTime = deltaTime;

		//handle sfml events
		{
			this->pollEvents();
		}		

		//before render	
		{
			stateManager.currentState->evokeAll("beforeRender", &M_eventData);
			window->clear();
		}

		//render
		{
			//stateManager.currentState->evokeAll("onRender", &M_eventData);

			size_t size = stateManager.currentState->getEntitiesCount();
			Scriptable::Entity* entities = stateManager.currentState->getEntitiesSortedByLayer();

			for (int i = 0; i < size; i++) {
				entities->evokeAll("onRender", &M_eventData);
				entities++;
			}

			window->display();
		}

		//after render
		{
			stateManager.currentState->evokeAll("afterRender", &M_eventData);
		}
	}
}

void Game::initWindow() 
{
	this->window = new sf::RenderWindow(sf::VideoMode(800, 600), "scipp");
	this->window->setFramerateLimit(60);
}


void Game::initStates()
{
	stateManager.changeState(new InitState());
	stateManager.currentState->init();
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