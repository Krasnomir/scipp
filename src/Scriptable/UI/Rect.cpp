#include <Scriptable/UI/Rect.hpp>
#include <Util.hpp>
#include <Game.hpp>

namespace Scriptable::UI{
    Rect::Rect(sf::FloatRect rect){
        M_rect = rect;
        auto viewSize = Scipp::globalGame->window->getDefaultView().getSize();
        rect.width *= viewSize.x;

        rect.height *= viewSize.y;

        /*left,top points the center of the rectangle*/

        m_RenderComponent = new Components::RenderComponent(Util::CreateRectangle(rect));
        m_Point.offset = {rect.left, rect.top};
        
    }

    void Rect::onWindowResized(const EventData* data){
        
    }

}