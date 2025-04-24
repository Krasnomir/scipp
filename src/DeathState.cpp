#include <Scriptable/UI/TextObject.hpp>

#include <DeathState.hpp>
#include <Game.hpp>

DeathState::DeathState() {

}

struct test_uiobj : public Scriptable::UI::TextObject {
    test_uiobj() : TextObject("font") {
        setPosition({100,100});
        setString("hello world");
        setVisible(true);
    }
};

void DeathState::init() {
    Scriptable::UI::TextObject::loadFont("RobotoMono.ttf", "font");

    initCamera(sf::Vector2f(Scipp::globalGame->window->getSize().x, Scipp::globalGame->window->getSize().y));
    Scipp::globalGame->stateManager.currentState->addUIObject<test_uiobj>("texttest");
}

void DeathState::onWindowClosed(const Scriptable::EventData* data)
{
	data->targetWindow->close();
}

void DeathState::onWindowResized(const Scriptable::EventData* data){
	Camera newCamera((sf::Vector2f)data->targetWindow->getSize(), data->currentState->M_camera.getPosition(), sf::FloatRect({0,0,1,1}));
	M_camera = newCamera;
	M_camera.apply();
}