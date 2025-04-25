#include <Scriptable/Components/TextComonent.hpp>

namespace Scriptable::Components {

    TextComponent::TextComponent() {

    }

    bool TextComponent::setLoadedFont(const std::string& fontName) {
        const sf::Font* loadedFont = (sf::Font*)Scriptable::UI::TextObject::getLoadedFont(fontName);
        if (!loadedFont) return false;
        setFont(*loadedFont);
        return true;
    }

    void TextComponent::onRender(const EventData* data) {
        data->targetWindow->draw(*this);
    }

    TextComponent::~TextComponent() {
    }

}
