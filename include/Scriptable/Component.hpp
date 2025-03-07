#ifndef __COMPONENTS
#define __COMPONENTS 1

#include <Scriptable/EventObject.hpp>

namespace Scriptable{
    class Component : public EventObject {
    public:
        Component();

        //some component-specific functions

        virtual ~Component();

        void* parentEntity = 0;

    };

    template<class T>
    concept DerivedComponent = std::is_base_of_v<Component, T>;

}

#endif