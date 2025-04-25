#ifndef __TEXT_COMPONENT_HPP__
#define __TEXT_COMPONENT_HPP__

#include <functional>
#include <SFML/Graphics/Text.hpp>
#include <Scriptable/Component.hpp>
#include <Scriptable/UI/TextObject.hpp>

namespace Scriptable::Components {

    class TextComponent : public Component, public sf::Text {
    public:
        TextComponent();

        bool setLoadedFont(const std::string& fontName);

        void onRender(const EventData* data) override;

        virtual ~TextComponent();
    };

}

#endif // __TEXT_COMPONENT_HPP__
