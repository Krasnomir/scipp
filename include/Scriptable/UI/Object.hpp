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
                CENTER = 0, TOPLEFT, TOPRIGHT, BOTTOMLEFT, BOTTOMRIGHT
            } point = AttachmentPlace::CENTER;
            sf::Vector2f offset;
        };
        
        //if objectName is null or invalid returns false and sets the global window as a constraint target.
        bool setAttachmentTarget(const std::string& objectName, AttachmentPoint point = {AttachmentPoint::CENTER, {0,0}});

        void set_attachment_point(const AttachmentPoint::AttachmentPlace& point);
        void set_attachment_offset(const sf::Vector2f& offset);

        sf::Vector2f update_position(const sf::Vector2f& size);

        virtual void draw_to_screen(Scriptable::EventData* e);

        int getLayer() const;
        void setLayer(int layer);

        void setVisible(bool visible);
        bool getVisible() const;

        void setGroup(const std::string& groupName);
        const std::string& getGroup();
        
    protected:
        Components::RenderComponent* m_RenderComponent = 0;
        AttachmentPoint m_Point;
        AttachmentPoint m_Source = {AttachmentPoint::CENTER, {0,0}};

        std::string m_Group = "";

    private:
        std::string m_Target = "";

        bool m_Visible = 0;
        int m_Layer = 0;
};

    template<class T>
    concept DerivedUIObject = std::is_base_of_v<Object, T>;
}   

#endif