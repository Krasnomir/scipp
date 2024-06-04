#include <Scriptable/Component.hpp>
#include <Scriptable/Components/TextureComponent.hpp>
#include <vector>
#include <SFML/Graphics.hpp>

#include <iostream>

namespace Scriptable::Components
{
    TextureComponent::TextureComponent(int i) {
        std::cout << "test" << std::endl;
    }

    TextureComponent::TextureComponent(){
        abort();
    }
}

