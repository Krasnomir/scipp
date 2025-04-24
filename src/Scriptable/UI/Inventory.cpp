#include <Scriptable/UI/Inventory.hpp>

namespace Scriptable::UI
{
    // Constructor for InventoryEntry
    InventoryEntry::InventoryEntry() : Rect({{0, 0}, {0.1, 0.1}})
    {
        setVisible(false);
        m_RenderComponent->setColor(sf::Color(255, 255, 255, 150));
        setLayer(2);
        setGroup("inventory");
    }

    // Constructor for InventoryFrame
    InventoryFrame::InventoryFrame() : Rect({{0, 0}, {0.7, 0.7}})
    {
        setGroup("inventory");
        setVisible(false);
        setLayer(1);
        this->m_RenderComponent->setColor(sf::Color(70, 70, 70, 150));

        // Add InventoryEntries to the game state
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

    // Show the inventory by setting the positions and visibility
    void InventoryFrame::show()
    {
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

        auto invGroup = Scipp::globalGame->stateManager.currentState->getObjectsByGroup("inventory");
        for(auto * invObj : invGroup){
            invObj->setVisible(true);
        }

    }

    // Hide the inventory by hiding all the entries and the frame
    void InventoryFrame::hide()
    {
        auto invGroup = Scipp::globalGame->stateManager.currentState->getObjectsByGroup("inventory");
        for(auto * invObj : invGroup){
            invObj->setVisible(false);
        }
    }

} // namespace Scriptable::UI
