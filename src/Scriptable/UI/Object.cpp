#include <Scriptable/UI/Rect.hpp>
#include <Scriptable/State.hpp>
#include <Util.hpp>
#include <Game.hpp>

namespace Scriptable::UI{
    Object::~Object(){
        if(m_RenderComponent) delete m_RenderComponent;
    }


    void Object::draw_to_screen(Scriptable::EventData* data){
        if(m_RenderComponent){
            data->targetWindow->draw(*m_RenderComponent);
        }
    }

    
}