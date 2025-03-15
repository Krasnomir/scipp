#pragma once
#ifndef _UIOBJECT_HPP
#define _UIOBJECT_HPP 1

#include <Scriptable/EventObject.hpp>
#include <Scriptable/Entity.hpp>
#include <Scriptable/Component.hpp>
#include <Scriptable/Components/RenderComponent.hpp>

#include <SFML/Graphics.hpp>



namespace Scriptable::UI
{	
    class Object : public EventObject{
    public:
        Object() = default;

        virtual ~Object();

        friend class State;

        virtual void draw_to_screen(Scriptable::EventData* e);
        
    protected:
        Components::RenderComponent* m_RenderComponent = 0;
    private:
    };

    template<class T>
    concept DerivedUIObject = std::is_base_of_v<Object, T>;
}   

#endif