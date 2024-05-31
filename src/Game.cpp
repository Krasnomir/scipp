#include <Game.h>
#include <StateManager.h>
#include <Scriptable/State.hpp>
#include <InitState.h>
#include <Scriptable/Entity.hpp>

#include <SFML/Graphics.hpp>
#include <Scriptable/EventObject.hpp>
#include <Scriptable/Components/SFMLRenderComponent.hpp>


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
	sf::Texture tTexture;

	DebugEntity(){
		addComponent<Scriptable::Components::SFMLRenderComponent>(std::vector<sf::Vector2f>({ {0,0}, {0,100}, {100, 0}, {100, 100}, {200, 50}, {150, 150} }));
		getComponent<Scriptable::Components::SFMLRenderComponent>()->setOrigin(getComponent<Scriptable::Components::SFMLRenderComponent>()->center());
		//tTexture.loadFromFile("test.png");
		
		//getComponent<Scriptable::Components::SFMLRenderComponent>()->setTexture(&tTexture);

	}

	void beforeRender(const Scriptable::EventData* data)
	{

	}

    void afterRender(const Scriptable::EventData* data)
	{

	} 

	void onMouseMoved(const Scriptable::EventData* data)
	{
		getComponent<Scriptable::Components::SFMLRenderComponent>()->setPosition(Scipp::globalGame->stateManager.currentState->M_camera.getMousePositionRelativeToCamera());
		//getComponent<Scriptable::Components::SFMLRenderComponent>()->setPosition((float) data->sfmlEvent.mouseMove.x, (float) data->sfmlEvent.mouseMove.y);
	}

	void onKeyPressed(const Scriptable::EventData* data)
	{
		auto* renderComponent = getComponent<Scriptable::Components::SFMLRenderComponent>();
		if(data->sfmlEvent.key.code == sf::Keyboard::Key::E){
			renderComponent->rotate(360 / 10.f);
		}
		else if(data->sfmlEvent.key.code == sf::Keyboard::Key::Q){

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