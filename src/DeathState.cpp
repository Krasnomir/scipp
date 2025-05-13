#include <Scriptable/UI/TextObject.hpp>
#include <Scriptable/UI/Rect.hpp>

#include <Scriptable/Entities/SimpleEntity.hpp>

#include <GameState.hpp>
#include <DeathState.hpp>
#include <Game.hpp>

#include <iostream>

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

    void onMouseButtonPressed(const Scriptable::EventData* data) {
        auto mouse_pos = data->currentState->getCamera().getMousePositionRelativeToCamera();
        auto vertices = this->m_RenderComponent->getVertices();

        for(size_t i=0; i+2<vertices.getVertexCount(); ++i) {
            auto pos1 = sf::Vector2f(vertices[i].position.x, vertices[i].position.y);
            auto pos2 = sf::Vector2f(vertices[i+1].position.x, vertices[i+1].position.y);
            auto pos3 = sf::Vector2f(vertices[i+2].position.x, vertices[i+2].position.y);

            auto triangle = Util::Triangle(pos1, pos2, pos3);

            if(Util::isInTriangle(triangle, mouse_pos)) {
                Scipp::globalGame->stateManager.scheduleStateChange(new GameState());
                return;
            }
        }
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