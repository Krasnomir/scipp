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

		case sf::Event::Resized:{
			stateManager.currentState->evokeAll("onWindowResized", &M_eventData);
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
			window->clear(sf::Color(90, 90, 75));
		}

		//render
		{
			stateManager.currentState->evokeAll("onRender", &M_eventData);
		}

		//ui render

		sf::View current_view = this->window->getView();
		this->window->setView(this->window->getDefaultView());

		{
			stateManager.currentState->evokeUIDraw(this->window);
		}

		this->window->setView(current_view);


		window->display();


		//after render
		{
			stateManager.currentState->evokeAll("afterRender", &M_eventData);
		}

		//execute scheduled deletion of components,entities, ...
		stateManager.currentState->exec_schd_deletion();

		stateManager.handleScheduledStateChanges();

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
	srand(time(NULL));

	this->initWindow();
	this->initStates();
}

Game::Game() 
{
	this->init();
	
	this->run();
}