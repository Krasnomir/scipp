#ifndef __TEXTURE_COMPONENT
#define __TEXTURE_COMPONENT 1

#include <Scriptable/Component.hpp>
#include <vector>

#include <SFML/Graphics.hpp>

namespace Scriptable::Components {

    class TextureComponent : public Component {
    public:
        TextureComponent(int i);
        ~TextureComponent() = default;
    };
}


#endif