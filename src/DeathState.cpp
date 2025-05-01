#include <Scriptable/UI/TextObject.hpp>
#include <Scriptable/UI/Rect.hpp>

#include <Scriptable/Entities/SimpleEntity.hpp>

#include <DeathState.hpp>
#include <Game.hpp>

DeathState::DeathState() {

}

struct death_text : public Scriptable::UI::TextObject {
    death_text() : TextObject("font") {
        setString("You died!");
        setVisible(true);
        set_attachment_offset(sf::Vector2f(0, -50));
        set_attachment_point(Scriptable::UI::Object::AttachmentPoint::CENTER);
    }
};

struct testbtn : public Scriptable::UI::Rect {
    testbtn(const sf::FloatRect& rect) : Rect(rect) {
        set_attachment_point(Scriptable::UI::Object::AttachmentPoint::CENTER);
        setVisible(true);
        m_RenderComponent->setColor(sf::Color(255, 255, 255, 150));
        setLayer(2);
    }
};

void DeathState::init() {
    Scriptable::UI::TextObject::loadFont("RobotoMono.ttf", "font");

    initCamera(sf::Vector2f(Scipp::globalGame->window->getSize().x, Scipp::globalGame->window->getSize().y));

    Scipp::globalGame->stateManager.currentState->addUIObject<death_text>("ui_death_text");

    Scipp::globalGame->stateManager.currentState->addUIObject<testbtn>("buttontest", sf::FloatRect(0,0,100,30));
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