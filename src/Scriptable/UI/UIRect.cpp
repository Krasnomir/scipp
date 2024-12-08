#include <Scriptable/UI/UIRect.hpp>

namespace Scriptable::UI{

    UIRect::UIRect(sf::FloatRect rect){
        m_RenderComponent = new Components::RenderComponent(Util::CreateRectangle(rect));
    }

}