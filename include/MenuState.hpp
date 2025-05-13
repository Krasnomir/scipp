#ifndef __MENUSTATE_HPP
#define __MENUSTATE_HPP 1

#include <Scriptable/State.hpp>
#include <Scriptable/UI/UI.hpp>
#include <Scriptable/UI/Rect.hpp>
#include <Scriptable/EventObject.hpp>
#include <Scriptable/UI/Menu.hpp>


class MenuState : public Scriptable::State{
public:
    MenuState(){}

    void init(){
        
        this->addUIObject<Scriptable::UI::MenuFrame>("menu_frame");
        
    }
    
    virtual inline void onWindowClosed(const Scriptable::EventData* data) {
        data->targetWindow->close();
    } 
    
    virtual ~MenuState() = default;
};


#endif