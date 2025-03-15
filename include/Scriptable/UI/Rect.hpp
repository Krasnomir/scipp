#pragma once

#ifndef _UIRECT_HPP
#define _UIRECT_HPP 1

#include <Scriptable/UI/Object.hpp>

namespace Scriptable::UI
{	

    class Rect : public Object{
    public:
        Rect(sf::FloatRect rectangle);

        virtual ~Rect() = default; 

        virtual void onWindowResized(const EventData* data) override;

    protected:
        sf::FloatRect M_rect;
    };

}

#endif