#include <Scriptable/UI/Rect.hpp>
#include <Util.hpp>
#include <Game.hpp>

namespace Scriptable::UI{
    Rect::Rect(sf::FloatRect rect){
        M_rect = rect;
        auto viewSize = Scipp::globalGame->window->getDefaultView().getSize();
        rect.width *= viewSize.x;
        rect.left *= viewSize.x;

        rect.top *= viewSize.y;
        rect.height *= viewSize.y;

        /*left,top points the center of the rectangle*/

        m_RenderComponent = new Components::RenderComponent(Util::CreateRectangle(rect));
        sf::Vector2f pos = {rect.left, rect.top};
        m_RenderComponent->setPosition(pos);
    }

    void Rect::onWindowResized(const EventData* data){
        
    }

}