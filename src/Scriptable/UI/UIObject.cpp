#include <Scriptable/UI/UIRect.hpp>
#include <Scriptable/State.hpp>
#include <Util.hpp>
#include <Game.hpp>

namespace Scriptable::UI{
    UIObject::~UIObject(){
        if(m_RenderComponent) delete m_RenderComponent;
    }

    //TODO: Onclick nie dziala

    void UIObject::onMouseButtonPressed(const Scriptable::EventData* data){
        if(m_RenderComponent && m_RenderComponent->isPointInside(Scipp::globalGame->stateManager.currentState->M_camera.getMousePositionRelativeToCamera(false))){
            onClick();
        }
    }
    

    void UIObject::draw_to_screen(Scriptable::EventData* data){
        if(m_RenderComponent){
            data->targetWindow->draw(*m_RenderComponent);
        }

        data->targetWindow->draw(Text);
    }
}