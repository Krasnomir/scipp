#include <Scriptable/UI/UIRect.hpp>

namespace Scriptable::UI{
    UIObject::~UIObject(){
        if(m_RenderComponent) delete m_RenderComponent;
    }

    void UIObject::draw_to_screen(Scriptable::EventData* data){
        if(m_RenderComponent){
            data->targetWindow->draw(*m_RenderComponent);
        }
    }
}