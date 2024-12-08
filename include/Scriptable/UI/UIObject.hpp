#pragma once

#include <Scriptable/EventObject.hpp>
#include <Scriptable/Entity.hpp>
#include <Scriptable/Component.hpp>
#include <Scriptable/Components/RenderComponent.hpp>

#include <SFML/Graphics.hpp>

namespace Scriptable::UI
{	
    class UIObject : public EventObject{
    public:
        UIObject() = default;

        virtual ~UIObject();

        friend class State;

        void draw_to_screen(Scriptable::EventData* e);
    protected:
        Components::RenderComponent* m_RenderComponent = 0;
        // TODO add text support sf::Text M_text; 
    private:
    };

    template<class T>
    concept DerivedUIObject = std::is_base_of_v<UIObject, T>;
}   