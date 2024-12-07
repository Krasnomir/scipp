#pragma once
#include <Scriptable/UI/UIObject.hpp>

namespace Scriptable::UI
{	

    class UIRect : public UIObject{
    public:
        UIRect(sf::FloatRect rectangle);

        ~UIRect() = default; 
    };

}