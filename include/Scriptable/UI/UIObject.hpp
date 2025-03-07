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

        virtual void onMouseButtonPressed(const EventData* data) override;

        virtual void onClick(){};

        void draw_to_screen(Scriptable::EventData* e);
        
        sf::Text Text;
    protected:
        Components::RenderComponent* m_RenderComponent = 0;
    private:
    };

    template<class T>
    concept DerivedUIObject = std::is_base_of_v<UIObject, T>;
}   