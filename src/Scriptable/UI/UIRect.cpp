#include <Scriptable/UI/UIRect.hpp>
#include <Util.hpp>
#include <Game.hpp>

namespace Scriptable::UI{
    UIRect::UIRect(sf::FloatRect rect){
        M_rect = rect;
        auto viewSize = Scipp::globalGame->window->getView().getSize();
        rect.width *= viewSize.x;
        rect.left *= viewSize.x;

        rect.height *= viewSize.y;
        rect.top *= viewSize.y;


        m_RenderComponent = new Components::RenderComponent(Util::CreateRectangle(rect));
    }

    void UIRect::onWindowResized(const EventData* data){
        delete m_RenderComponent;

        auto viewSize = Scipp::globalGame->window->getView().getSize();
        auto rect = M_rect;
        rect.width *= viewSize.x;
        rect.left *= viewSize.x;

        rect.height *= viewSize.y;
        rect.top *= viewSize.y;
        m_RenderComponent = new Components::RenderComponent(Util::CreateRectangle(rect));
    }


}