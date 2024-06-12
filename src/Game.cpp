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

struct ProjectileEntity : public Scriptable::Entity
{
	double M_angle;

	ProjectileEntity(double angle, sf::Vector2f pos) : M_angle(angle){
		addComponent<Scriptable::Components::RenderComponent>(std::vector<sf::Vector2f>({{0,50}, {0, 0}, {25,50}, {25, 0}}));
		getComponent<Scriptable::Components::RenderComponent>()->setOrigin(getComponent<Scriptable::Components::RenderComponent>()->center());
		getComponent<Scriptable::Components::RenderComponent>()->setPosition(pos);

		getComponent<Scriptable::Components::RenderComponent>()->setRotation(M_angle + 90.f);
	}

	void beforeRender(const Scriptable::EventData* data)
	{
		auto* renderC = this->getComponent<Scriptable::Components::RenderComponent>();
		
		float angle = M_angle;

		renderC->setPosition(Util::movePoint(renderC->getPosition(), 5, angle));

	}

    void afterRender(const Scriptable::EventData* data)
	{
		
	} 

};

struct DebugEntity : public Scriptable::Entity
{
	DebugEntity(){
		addComponent<Scriptable::Components::RenderComponent>(std::vector<sf::Vector2f>({{0,50}, {0, 0}, {50,50}, {50, 0}}));
		getComponent<Scriptable::Components::RenderComponent>()->setOrigin(getComponent<Scriptable::Components::RenderComponent>()->center());
		
		getComponent<Scriptable::Components::RenderComponent>()->addCostume("test", "test.png");
		getComponent<Scriptable::Components::RenderComponent>()->loadCostume("test");
	}

	void beforeRender(const Scriptable::EventData* data)
	{
		auto* renderC = this->getComponent<Scriptable::Components::RenderComponent>();
		auto mousePos = Scipp::globalGame->stateManager.currentState->M_camera.getMousePositionRelativeToCamera();
		

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && Util::getDistanceBetweenPoints(renderC->getPosition(), mousePos) > 10) {
			float angle = renderC->getRotation();


			renderC->setPosition(Util::movePoint(renderC->getPosition(), 2, angle));
		}

	}

    void afterRender(const Scriptable::EventData* data)
	{
		
	} 

	void onMouseMoved(const Scriptable::EventData* data)
	{
		auto mouse_pos = Scipp::globalGame->stateManager.currentState->M_camera.getMousePositionRelativeToCamera();
		auto* renderComponent = getComponent<Scriptable::Components::RenderComponent>();

		renderComponent->setRotation(Util::getAngleBetweenPoints(renderComponent->getPosition(), mouse_pos));
	}

	void onMouseButtonPressed(const Scriptable::EventData* data){
		if(data->sfmlEvent.mouseButton.button == sf::Mouse::Button::Left){
			static uint32_t proj_ID = 0;

			Scipp::globalGame->stateManager.currentState->addEntity<ProjectileEntity>(std::to_string(proj_ID), getComponent<Scriptable::Components::RenderComponent>()->getRotation(), getComponent<Scriptable::Components::RenderComponent>()->getPosition());
			proj_ID++;
		}
	}

	void onKeyPressed(const Scriptable::EventData* data)
	{

		
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