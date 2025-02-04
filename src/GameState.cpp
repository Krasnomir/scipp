#include <StateManager.hpp>
#include <Scriptable/State.hpp>
#include <GameState.hpp>
#include <Scriptable/Entity.hpp>
#include <Camera.hpp>
#include <Scriptable/Components/RenderComponent.hpp>
#include <Scriptable/Components/LifetimeComponent.hpp>
#include <Scriptable/Components/PhysicsComponent.hpp>
#include <Util.hpp>
#include <Game.hpp>

#include <SFML/Graphics.hpp>

#include <iostream>

struct ProjectileEntity : public Scriptable::Entity
{
	double M_angle;

	static void testr() {
		std::cout << "ELAPSED" << std::endl;
	}

	ProjectileEntity(double angle, sf::Vector2f pos) : M_angle(angle) {
		addComponent<Scriptable::Components::RenderComponent>(std::vector<sf::Vector2f>({ {0,50}, {50,50}, {25,0}}));
		addComponent<Scriptable::Components::LifetimeComponent>(sf::seconds(2.f), testr);	
		//addComponent<Scriptable::Components::PhysicsComponent>(5, this->getComponent<Scriptable::Components::RenderComponent>());
		getComponent<Scriptable::Components::RenderComponent>()->setOrigin(getComponent<Scriptable::Components::RenderComponent>()->center());
		getComponent<Scriptable::Components::RenderComponent>()->setPosition(pos);
		getComponent<Scriptable::Components::RenderComponent>()->setRotation(M_angle);
	}

	void beforeRender(const Scriptable::EventData* data)
	{
		auto* renderC = this->getComponent<Scriptable::Components::RenderComponent>();

		float angle = M_angle;

		// renderC->setPosition(Util::movePoint(renderC->getPosition(), 0.5, angle));

		Scriptable::Entity* debugEntity = Scipp::globalGame->stateManager.currentState->getEntity("test1");
		Scriptable::Components::RenderComponent* debugEntityRC = debugEntity->getComponent<Scriptable::Components::RenderComponent>();

		/*
		if(debugEntityRC->boundingBoxCollide(renderC)) {
			getComponent<Scriptable::Components::LifetimeComponent>()->restart(sf::seconds(2.f));
			renderC->setRotation(renderC->getRotation() + 20);
		}
		*/

	}

	void afterRender(const Scriptable::EventData* data)
	{
		
	}

};

struct DebugEntity : public Scriptable::Entity
{
	DebugEntity() {
		// addComponent<Scriptable::Components::RenderComponent>(std::vector<sf::Vector2f>({{0,0}, {0, 100}, {30, 0}, 	{30,0}, {30, 100}, {0,100}}));

		addComponent<Scriptable::Components::RenderComponent>(std::vector<std::pair<sf::Vector2f, sf::Vector2f>>({{{0,0}, {18,70}}, {{0, 100}, {18, 170}}, {{30, 0},  {48, 70}}, {{30,0}, {48, 70}}, {{30,100}, {48, 170}},{{0,100},{18, 170}}}));
		addComponent<Scriptable::Components::PhysicsComponent>(0, getComponent<Scriptable::Components::RenderComponent>());

		getComponent<Scriptable::Components::RenderComponent>()->setOrigin(getComponent<Scriptable::Components::RenderComponent>()->center());

		getComponent<Scriptable::Components::RenderComponent>()->addCostume("test", "test.png", sf::IntRect({0,0, 398, 273}));
		getComponent<Scriptable::Components::RenderComponent>()->loadCostume("test");

	}

	void beforeRender(const Scriptable::EventData* data)
	{

		auto* renderC = this->getComponent<Scriptable::Components::RenderComponent>();
		auto mousePos = Scipp::globalGame->stateManager.currentState->M_camera.getMousePositionRelativeToCamera(true);

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)){
			
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && Util::getDistanceBetweenPoints(renderC->getPosition(), mousePos) > 10) {
			/*
			float angle = renderC->getRotation();

			renderC->setPosition(Util::movePoint(renderC->getPosition(), data->deltaTime.asMilliseconds(), angle));
			*/
			getComponent<Scriptable::Components::PhysicsComponent>()->velocity = 5;
		}
		else {
			getComponent<Scriptable::Components::PhysicsComponent>()->velocity = 0;
		}
	}

	void afterRender(const Scriptable::EventData* data)
	{
		
	}

	void onMouseMoved(const Scriptable::EventData* data)
	{
		auto mouse_pos = Scipp::globalGame->stateManager.currentState->M_camera.getMousePositionRelativeToCamera(true);
		auto* renderComponent = getComponent<Scriptable::Components::RenderComponent>();

		renderComponent->setRotation(Util::getAngleBetweenPoints(renderComponent->getPosition(), mouse_pos));
	}

	void onMouseButtonPressed(const Scriptable::EventData* data) {
		if (data->sfmlEvent.mouseButton.button == sf::Mouse::Button::Left) {
			static uint32_t proj_ID = 0;

			sf::Vector2f bulletStartPosition = Util::movePoint(getComponent<Scriptable::Components::RenderComponent>()->getPosition(), 50, getComponent<Scriptable::Components::RenderComponent>()->getRotation());

			Scipp::globalGame->stateManager.currentState->addEntity<ProjectileEntity>(std::to_string(proj_ID), getComponent<Scriptable::Components::RenderComponent>()->getRotation(), bulletStartPosition);
			proj_ID++;
		}
	}

	void onKeyPressed(const Scriptable::EventData* data)
	{


	}

};

void GameState::onWindowClosed(const Scriptable::EventData* data)
{
	data->targetWindow->close();
}

void GameState::onWindowResized(const Scriptable::EventData* data){
	Camera newCamera((sf::Vector2f)data->targetWindow->getSize(), data->currentState->M_camera.getPosition(), sf::FloatRect({0,0,1,1}));
	M_camera = newCamera;
	M_camera.apply();
}

void GameState::cameraFollow() {
	sf::Vector2f playerPosition = getEntity("test1")->getComponent<Scriptable::Components::RenderComponent>()->getPosition();
	// Camera position will be slightly shifted to where the player is looking (where the mouse cursor is) it also depends on how far from center it is 
	float shiftIntensity = 20; // The smaller the number the bigger the intensity is going to be
	float xShift = M_camera.getMousePositionRelativeToCamera().x / shiftIntensity;
	float yShift = M_camera.getMousePositionRelativeToCamera().y / shiftIntensity;

	M_camera.setPosition(sf::Vector2f(playerPosition.x + xShift, playerPosition.y + yShift));
	M_camera.apply();
}

void GameState::onRender(const Scriptable::EventData* data) {
	cameraFollow();
}

GameState::GameState()
{
	initCamera();
}

#include <Scriptable/UI/UIRect.hpp>

void GameState::init()
{
	Scipp::globalGame->stateManager.currentState->addUIObject<Scriptable::UI::UIRect>("Hello", sf::FloatRect({0,0, 100, 50}));
	
	Scipp::globalGame->stateManager.currentState->addEntity<DebugEntity>("test1");
}