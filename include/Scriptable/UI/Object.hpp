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

        
        struct AttachmentPoint{
            enum AttachmentPlace : uint8_t{
                TOPLEFT, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT, CENTER
            } point;
            sf::Vector2f offset;
        };
        
        //if objectName is null or invalid returns false and sets the global window as a constraint target.
        bool setAttachmentTarget(const std::string& objectName, AttachmentPoint point = {AttachmentPoint::CENTER, {0,0}});

        virtual void draw_to_screen(Scriptable::EventData* e);

        
    protected:
        Components::RenderComponent* m_RenderComponent = 0;
    private:
        std::string m_Target = "";
        AttachmentPoint m_Point;
};

    template<class T>
    concept DerivedUIObject = std::is_base_of_v<Object, T>;
}   

#endif