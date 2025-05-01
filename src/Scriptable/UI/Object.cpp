#include <Scriptable/UI/Rect.hpp>
#include <Scriptable/State.hpp>
#include <Util.hpp>
#include <Game.hpp>
#include <Input/Input.hpp>

#include <iostream>

namespace Scriptable::UI{
    Object::~Object(){
        if(m_RenderComponent) delete m_RenderComponent;
    }

    void Object::setLayer(int Layer){
        m_Layer = Layer;
    }
    
    int Object::getLayer() const{
        return m_Layer;
    }

    void Object::setVisible(bool visible){
        m_Visible = visible;
    }

    bool Object::getVisible() const{
        return m_Visible;
    }
    
    void Object::setGroup(const std::string& groupName){
        m_Group = groupName;
    }

    const std::string& Object::getGroup(){
        return m_Group;
    }

    bool Object::setAttachmentTarget(const std::string& target, AttachmentPoint point){
        m_Point = point;
        if(target == "" || !Scipp::globalGame->stateManager.currentState->hasUIObject(target)){
            m_Target = "";
            return false;
        }
        else{
            m_Target = target;
            return true;
        }
    }

    void Object::set_attachment_point(const AttachmentPoint::AttachmentPlace& point) {
        m_Point.point = point;
    }

    void Object::set_attachment_offset(const sf::Vector2f& offset) {
        m_Point.offset = offset;
    }

    sf::Vector2f Object::update_position(const sf::Vector2f& size) {
        sf::Vector2f window_size = (sf::Vector2f) Scipp::globalGame->window->getSize();
        sf::Vector2f camera_position = Scipp::globalGame->stateManager.currentState->getCamera().getPosition();

        sf::Vector2f new_position;

        switch(m_Point.point){
            case Object::AttachmentPoint::TOPLEFT:{
                new_position.x = -(window_size.x / 2) + m_Point.offset.x;
                new_position.y = -(window_size.y / 2) + m_Point.offset.y;
                break;
            }
            case Object::AttachmentPoint::TOPRIGHT:{
                new_position.x = (window_size.x / 2) - size.x + m_Point.offset.x;
                new_position.y = -(window_size.y / 2) + m_Point.offset.y;
                break;
            }
            case Object::AttachmentPoint::BOTTOMLEFT:{
                new_position.x = -(window_size.x / 2) + m_Point.offset.x;
                new_position.y = (window_size.y / 2) - size.y + m_Point.offset.y;
                break;
            }
            case Object::AttachmentPoint::BOTTOMRIGHT:{
                new_position.x = (window_size.x / 2) - size.x + m_Point.offset.x;
                new_position.y = (window_size.y / 2) - size.y + m_Point.offset.y;
                break;
            }
            case Object::AttachmentPoint::CENTER:{
                new_position.x = -size.x / 2 + m_Point.offset.x;
                new_position.y = -size.y / 2 + m_Point.offset.y;
                break;
            }
        }

        new_position += camera_position;

        if(m_RenderComponent) m_RenderComponent->setPosition(new_position);

        return new_position;
    }

    void Object::draw_to_screen(Scriptable::EventData* data){
        
        if(m_RenderComponent) {
            m_RenderComponent->setPosition(update_position(sf::Vector2f(m_RenderComponent->getBoundingBox().width, m_RenderComponent->getBoundingBox().height)));
            if(m_Visible) data->targetWindow->draw(*m_RenderComponent);
        }
        /*
        if(m_Target != "" && Scipp::globalGame->stateManager.currentState->hasUIObject(m_Target)){
            Object* target = Scipp::globalGame->stateManager.currentState->getUIObject(m_Target);
            sf::FloatRect aabb = target->m_RenderComponent->getBoundingBox();

            sf::Vector2f center = target->m_RenderComponent->getPosition();
            sf::Vector2f targetPoint;

            switch(m_Point.point){
                case Object::AttachmentPoint::TOPLEFT:{
                    targetPoint = {center.x - (aabb.width / 2.f), center.y - (aabb.height / 2.f)};
                    break;
                }
                case Object::AttachmentPoint::TOPRIGHT:{
                    targetPoint = {center.x + (aabb.width / 2.f), center.y - (aabb.height / 2.f)};

                    break;
                }
                case Object::AttachmentPoint::BOTTOMLEFT:{
                    targetPoint = {center.x - (aabb.width / 2.f), center.y + (aabb.height / 2.f)};
                    break;
                }
                case Object::AttachmentPoint::BOTTOMRIGHT:{
                    targetPoint = {center.x + (aabb.width / 2.f), center.y + (aabb.height / 2.f)};
                    break;
                }
                case Object::AttachmentPoint::CENTER:{
                    targetPoint = center;
                    break;
                }
            }
            targetPoint.x += (m_Point.offset.x * aabb.width);
            targetPoint.y += (m_Point.offset.y * aabb.height);
            if(m_RenderComponent) {
                m_RenderComponent->setPosition(targetPoint);
            }

        }
        else{
            auto camera = Scipp::globalGame->window->getDefaultView();
            sf::FloatRect aabb = {{0,0}, camera.getSize()};

            sf::Vector2f center = {aabb.width / 2.f, aabb.height / 2.f};
            sf::Vector2f targetPoint;

            switch(m_Point.point){
                case Object::AttachmentPoint::TOPLEFT:{
                    targetPoint = {center.x - (aabb.width / 2.f), center.y - (aabb.height / 2.f)};
                    break;
                }
                case Object::AttachmentPoint::TOPRIGHT:{
                    targetPoint = {center.x + (aabb.width / 2.f), center.y - (aabb.height / 2.f)};
                    break;
                }
                case Object::AttachmentPoint::BOTTOMLEFT:{
                    targetPoint = {center.x - (aabb.width / 2.f), center.y + (aabb.height / 2.f)};
                    break;
                }
                case Object::AttachmentPoint::BOTTOMRIGHT:{
                    targetPoint = {center.x + (aabb.width / 2.f), center.y + (aabb.height / 2.f)};
                    break;
                }
                case Object::AttachmentPoint::CENTER:{
                    targetPoint = center;
                    break;
                }
            }
            targetPoint.x += (m_Point.offset.x * aabb.width);
            targetPoint.y += (m_Point.offset.y * aabb.height);
            if(m_RenderComponent) {
                m_RenderComponent->setPosition(targetPoint);
            }
        }
        if(m_RenderComponent && m_Visible){
            data->targetWindow->draw(*m_RenderComponent);
            std::cout << data->targetWindow->getSize().x << "\n";
        }
        */
    }

    
}