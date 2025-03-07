#pragma once
#include <Scriptable/UI/UIObject.hpp>

namespace Scriptable::UI
{	

    class UIRect : public UIObject{
    public:
        UIRect(sf::FloatRect rectangle);

        virtual ~UIRect() = default; 

        virtual void onWindowResized(const EventData* data) override;


    protected:
        sf::FloatRect M_rect;
    };

}