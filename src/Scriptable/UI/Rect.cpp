#include <Scriptable/UI/Rect.hpp>
#include <Util.hpp>
#include <Game.hpp>

namespace Scriptable::UI{
    Rect::Rect(sf::FloatRect rect){
        M_rect = rect;
        /*
        auto viewSize = Scipp::globalGame->window->getDefaultView().getSize();
        rect.width *= viewSize.x;
        rect.left *= viewSize.x;

        rect.top *= viewSize.y;
        rect.height *= viewSize.y;

        left,top points the center of the rectangle
        */
        m_RenderComponent = new Components::RenderComponent(Util::CreateRectangle(rect));
        m_RenderComponent->setOrigin(sf::Vector2f(-rect.width/2, -rect.height/2)); // set origin to the top left corner
        /*
        sf::Vector2f pos = {rect.left, rect.top};
        m_Point.offset = {rect.left, rect.top};
        m_RenderComponent->setPosition(pos);
        */
    }

    void Rect::onWindowResized(const EventData* data){
        
    }

}