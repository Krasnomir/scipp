#include <Scriptable/UI/Rect.hpp>
#include <Scriptable/State.hpp>
#include <Util.hpp>
#include <Game.hpp>
#include <Input/Input.hpp>

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

    void Object::draw_to_screen(Scriptable::EventData* data){
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
            targetPoint.x += m_Point.offset.x;
            targetPoint.y += m_Point.offset.y;
            if(m_RenderComponent) {
                m_RenderComponent->setPosition(targetPoint);
            }

        }
        else{
            
        }

        if(m_RenderComponent){
            data->targetWindow->draw(*m_RenderComponent);
        }
    }

    
}