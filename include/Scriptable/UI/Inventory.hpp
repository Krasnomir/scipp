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
    // Inventory Entry Class
    class InventoryEntry : public Rect
    {
    public:
        InventoryEntry();
    };

    // Inventory Frame Class
    class InventoryFrame : public Rect
    {
    public:
        InventoryFrame();

        void show();
        void hide();
    };

} // namespace Scriptable::UI

#endif // __INVENTORY_HPP
