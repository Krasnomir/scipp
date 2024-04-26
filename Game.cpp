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

	for (sf::VertexArray va : this->components)
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

void Game::initComponents() 
{
	sf::VertexArray triangle(sf::TriangleStrip, 4);

	triangle[0].position = sf::Vector2f(0.f, 0.f);
	triangle[1].position = sf::Vector2f(0.f, 200.f);
	triangle[2].position = sf::Vector2f(200.f, 0.f);
	triangle[2].position = sf::Vector2f(200.f, 200.f);

	triangle[0].color = sf::Color::Yellow;
	triangle[1].color = sf::Color::Green;
	triangle[2].color = sf::Color::Red;
	triangle[3].color = sf::Color::Magenta;

	this->components.push_back(triangle);
}

void Game::initStates()
{
	this->stateManager = new StateManager(this);
	this->stateManager->changeState(new TestState(this->stateManager));
}

void Game::init() 
{
	this->initStates();
	this->initComponents();
	this->initWindow();
}

Game::Game() 
{
	this->init();
	this->run();
}