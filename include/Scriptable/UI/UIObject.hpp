#pragma once

#include <Scriptable/EventObject.hpp>
#include <Scriptable/Component.hpp>

#include <SFML/Graphics.hpp>

namespace Scriptable::UI
{	

    class UIElement : public EventObject{

        UIElement() = default;

        virtual ~UIElement() = default;

    };

}