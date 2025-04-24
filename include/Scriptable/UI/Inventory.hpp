#ifndef __INVENTORY_HPP
#define __INVENTORY_HPP


#include <Scriptable/EventObject.hpp>
#include <Scriptable/Entity.hpp>
#include <Scriptable/Component.hpp>
#include <Scriptable/Components/RenderComponent.hpp>
#include <Scriptable/UI/Rect.hpp>
#include <Scriptable/State.hpp>
#include <Game.hpp>

#include <SFML/Graphics.hpp>



namespace Scriptable::UI
{	

    class InventoryEntry : public Rect{
        public:
            InventoryEntry() : Rect({{0,0}, {0.1,0.1}}){
                setVisible(false);
                m_RenderComponent->setColor(sf::Color(255,255,255,150));
                setLayer(2);
            }

            
        };

    class InventoryFrame : public Rect{
    public:
        InventoryFrame() : Rect({{0,0}, {0.7,0.7}}){
            setVisible(false);
            setLayer(1);
            this->m_RenderComponent->setColor(sf::Color(70,70,70,150));
            Scipp::globalGame->stateManager.currentState->addUIObject<InventoryEntry>("0_inventory_entry0");
            Scipp::globalGame->stateManager.currentState->addUIObject<InventoryEntry>("0_inventory_entry1");
            Scipp::globalGame->stateManager.currentState->addUIObject<InventoryEntry>("0_inventory_entry2");
            Scipp::globalGame->stateManager.currentState->addUIObject<InventoryEntry>("0_inventory_entry3");
            Scipp::globalGame->stateManager.currentState->addUIObject<InventoryEntry>("0_inventory_entry4");

            Scipp::globalGame->stateManager.currentState->addUIObject<InventoryEntry>("1_inventory_entry0");
            Scipp::globalGame->stateManager.currentState->addUIObject<InventoryEntry>("1_inventory_entry1");
            Scipp::globalGame->stateManager.currentState->addUIObject<InventoryEntry>("1_inventory_entry2");
            Scipp::globalGame->stateManager.currentState->addUIObject<InventoryEntry>("1_inventory_entry3");
            Scipp::globalGame->stateManager.currentState->addUIObject<InventoryEntry>("1_inventory_entry4");
        }


        void show(){

            auto* obj = Scipp::globalGame->stateManager.currentState->getUIObject("0_inventory_entry0");
            obj->setAttachmentTarget("inventory_frame", {Object::AttachmentPoint::TOPLEFT, {0.1, 0.1}});

            obj = Scipp::globalGame->stateManager.currentState->getUIObject("0_inventory_entry1");
            obj->setAttachmentTarget("inventory_frame", {Object::AttachmentPoint::TOPLEFT, {0.3, 0.1}});

            obj = Scipp::globalGame->stateManager.currentState->getUIObject("0_inventory_entry2");
            obj->setAttachmentTarget("inventory_frame", {Object::AttachmentPoint::TOPLEFT, {0.5, 0.1}});

            obj = Scipp::globalGame->stateManager.currentState->getUIObject("0_inventory_entry3");
            obj->setAttachmentTarget("inventory_frame", {Object::AttachmentPoint::TOPLEFT, {0.7, 0.1}});

            obj = Scipp::globalGame->stateManager.currentState->getUIObject("0_inventory_entry4");
            obj->setAttachmentTarget("inventory_frame", {Object::AttachmentPoint::TOPLEFT, {0.9, 0.1}});

            obj = Scipp::globalGame->stateManager.currentState->getUIObject("1_inventory_entry0");
            obj->setAttachmentTarget("inventory_frame", {Object::AttachmentPoint::TOPLEFT, {0.1, 0.3}});

            obj = Scipp::globalGame->stateManager.currentState->getUIObject("1_inventory_entry1");
            obj->setAttachmentTarget("inventory_frame", {Object::AttachmentPoint::TOPLEFT, {0.3, 0.3}});

            obj = Scipp::globalGame->stateManager.currentState->getUIObject("1_inventory_entry2");
            obj->setAttachmentTarget("inventory_frame", {Object::AttachmentPoint::TOPLEFT, {0.5, 0.3}});

            obj = Scipp::globalGame->stateManager.currentState->getUIObject("1_inventory_entry3");
            obj->setAttachmentTarget("inventory_frame", {Object::AttachmentPoint::TOPLEFT, {0.7, 0.3}});

            obj = Scipp::globalGame->stateManager.currentState->getUIObject("1_inventory_entry4");
            obj->setAttachmentTarget("inventory_frame", {Object::AttachmentPoint::TOPLEFT, {0.9, 0.3}});

            size_t column_end = 4;
            size_t row_end = 1;
            for(size_t row = 0; row <= row_end; row++){
                for(size_t column = 0; column <= column_end; column++){
                    std::string inv_name = std::to_string(row) + "_inventory_entry" + std::to_string(column);
                    auto* obj = Scipp::globalGame->stateManager.currentState->getUIObject(inv_name);
                    obj->setVisible(true);

                }
            }
            setVisible(true);
        }

        void hide(){
            size_t column_end = 4;
            size_t row_end = 1;
            for(size_t row = 0; row <= row_end; row++){
                for(size_t column = 0; column <= column_end; column++){
                    std::string inv_name = std::to_string(row) + "_inventory_entry" + std::to_string(column);
                    auto* obj = Scipp::globalGame->stateManager.currentState->getUIObject(inv_name);
                    obj->setVisible(false);

                }
            }
            Scipp::globalGame->stateManager.currentState->getUIObject("inventory_frame")->setVisible(false);
            setVisible(false);
        }

    };

    

}

#endif