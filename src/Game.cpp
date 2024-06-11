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

#include <math.h>

//temporary
double getAngle(double x1, double y1, double x2, double y2) {
    // Calculate the difference in x and y coordinates
    double deltaX = x2 - x1;
    double deltaY = y2 - y1;
    
    // Calculate the angle in radians
    double angleRadians = atan2(deltaY, deltaX);
    
    // Convert the angle to degrees
    double angleDegrees = angleRadians * (180.0 / M_PI);
    
    // Ensure the angle is in the range [0, 360)
    if (angleDegrees < 0) {
        angleDegrees += 360;
    }

    return angleDegrees;
}

//temporary
sf::Vector2f movePoint(const sf::Vector2f& p, double distance, double angleDegrees) {
    // Convert the angle from degrees to radians
    double angleRadians = angleDegrees * M_PI / 180.0;

    // Calculate the new coordinates
    double newX = p.x + distance * cos(angleRadians);
    double newY = p.y + distance * sin(angleRadians);

    return { newX, newY };
}

//temporary
float getDistance(sf::Vector2f a, sf::Vector2f b){
	return sqrt(pow(a.x - b.x, 2) + pow((a.y - b.y), 2));
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

		renderC->setPosition(movePoint(renderC->getPosition(), 5, angle));

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
		

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && getDistance(renderC->getPosition(), mousePos) > 10) {
			float angle = renderC->getRotation();


			renderC->setPosition(movePoint(renderC->getPosition(), 2, angle));
		}

	}

    void afterRender(const Scriptable::EventData* data)
	{
		
	} 

	void onMouseMoved(const Scriptable::EventData* data)
	{
		auto mouse_pos = Scipp::globalGame->stateManager.currentState->M_camera.getMousePositionRelativeToCamera();
		auto* renderComponent = getComponent<Scriptable::Components::RenderComponent>();

		renderComponent->setRotation(getAngle(renderComponent->getPosition().x, renderComponent->getPosition().y, mouse_pos.x, mouse_pos.y));
		
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