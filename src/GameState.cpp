#include <StateManager.hpp>
#include <Scriptable/State.hpp>
#include <GameState.hpp>
#include <Scriptable/Entity.hpp>
#include <Camera.hpp>
#include <Scriptable/Components/RenderComponent.hpp>
#include <Util.hpp>
#include <Game.hpp>

#include <SFML/Graphics.hpp>

#include <iostream>

struct ProjectileEntity : public Scriptable::Entity
{
	double M_angle;

	ProjectileEntity(double angle, sf::Vector2f pos) : M_angle(angle) {
		addComponent<Scriptable::Components::RenderComponent>(std::vector<sf::Vector2f>({ {0,50}, {0, 0}, {25,50}, {25, 0} }));
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
	DebugEntity() {
		addComponent<Scriptable::Components::RenderComponent>(std::vector<sf::Vector2f>({ {0,50}, {0, 0}, {50,50}, {50, 0} }));
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

	void onMouseButtonPressed(const Scriptable::EventData* data) {
		if (data->sfmlEvent.mouseButton.button == sf::Mouse::Button::Left) {
			static uint32_t proj_ID = 0;

			Scipp::globalGame->stateManager.currentState->addEntity<ProjectileEntity>(std::to_string(proj_ID), getComponent<Scriptable::Components::RenderComponent>()->getRotation(), getComponent<Scriptable::Components::RenderComponent>()->getPosition());
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

GameState::GameState()
{
	initCamera();
}

void GameState::init()
{
	Scipp::globalGame->stateManager.currentState->addEntity<DebugEntity>("test1");
}