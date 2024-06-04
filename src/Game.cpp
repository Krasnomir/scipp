#include <Game.hpp>
#include <StateManager.hpp>
#include <Scriptable/State.hpp>
#include <InitState.hpp>
#include <Scriptable/Entity.hpp>
#include <Scriptable/EventObject.hpp>
#include <Scriptable/Components/RenderComponent.hpp>

#include <SFML/Graphics.hpp>

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

struct DebugEntity : public Scriptable::Entity
{
	DebugEntity(){
		sf::Texture tex1;
		sf::Texture tex2;
		tex1.loadFromFile("drewno.jpg");
		tex2.loadFromFile("de.jpg");

		addComponent<Scriptable::Components::RenderComponent>(std::vector<sf::Vector2f>({ {0,0}, {0,100}, {100, 0}, {100, 100}, {200, 50}, {150, 150} }));
		getComponent<Scriptable::Components::RenderComponent>()->setOrigin(getComponent<Scriptable::Components::RenderComponent>()->center());
		getComponent<Scriptable::Components::RenderComponent>()->addCostume("test", "de.jpg");
		getComponent<Scriptable::Components::RenderComponent>()->addCostume("test2", "drewno.jpg", sf::IntRect(0, 0, 20, 20));
		getComponent<Scriptable::Components::RenderComponent>()->loadCostume("test");
	}

	void beforeRender(const Scriptable::EventData* data)
	{

	}

    void afterRender(const Scriptable::EventData* data)
	{

	} 

	void onMouseMoved(const Scriptable::EventData* data)
	{
		getComponent<Scriptable::Components::RenderComponent>()->setPosition(Scipp::globalGame->stateManager.currentState->M_camera.getMousePositionRelativeToCamera());
		//getComponent<Scriptable::Components::RenderComponent>()->setPosition((float) data->sfmlEvent.mouseMove.x, (float) data->sfmlEvent.mouseMove.y);
	}

	void onKeyPressed(const Scriptable::EventData* data)
	{
		auto* renderComponent = getComponent<Scriptable::Components::RenderComponent>();
		if(data->sfmlEvent.key.code == sf::Keyboard::Key::E){
			getComponent<Scriptable::Components::RenderComponent>()->loadCostume("test");
			renderComponent->rotate(360 / 10.f);
		}
		else if(data->sfmlEvent.key.code == sf::Keyboard::Key::Q){
			getComponent<Scriptable::Components::RenderComponent>()->loadCostume("test2");
			renderComponent->rotate(-360 / 10.f);
		}
		else if (data->sfmlEvent.key.code == sf::Keyboard::Key::R) {
			float currentRotation = Scipp::globalGame->stateManager.currentState->M_camera.getRotation();
			Scipp::globalGame->stateManager.currentState->M_camera.setRotation(currentRotation + 5.0f);
			Scipp::globalGame->stateManager.currentState->M_camera.apply();
		}
		else if (data->sfmlEvent.key.code == sf::Keyboard::Key::W) {
			Scipp::globalGame->stateManager.currentState->M_camera.move(sf::Vector2f(0.f, -10.f));
			Scipp::globalGame->stateManager.currentState->M_camera.apply();
		}
		else if (data->sfmlEvent.key.code == sf::Keyboard::Key::A) {
			Scipp::globalGame->stateManager.currentState->M_camera.move(sf::Vector2f(-10.f, 0.f));
			Scipp::globalGame->stateManager.currentState->M_camera.apply();
		}
		else if (data->sfmlEvent.key.code == sf::Keyboard::Key::S) {
			Scipp::globalGame->stateManager.currentState->M_camera.move(sf::Vector2f(0.f, 10.f));
			Scipp::globalGame->stateManager.currentState->M_camera.apply();
		}
		else if (data->sfmlEvent.key.code == sf::Keyboard::Key::D) {
			Scipp::globalGame->stateManager.currentState->M_camera.move(sf::Vector2f(10.f, 0.f));
			Scipp::globalGame->stateManager.currentState->M_camera.apply();
		}
	}
};

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
			stateManager.currentState->evokeAll("onRender", &M_eventData);
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
}

void Game::init() 
{
	Scipp::globalGame = this;

	this->initWindow();
	this->initStates();

	stateManager.currentState->addEntity<DebugEntity>("test1");
}

Game::Game() 
{
	this->init();
	
	this->run();
}