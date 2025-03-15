#include <Scriptable/UI/TextObject.hpp>
#include <Scriptable/State.hpp>
#include <Util.hpp>
#include <Game.hpp>

namespace Scriptable::UI{
    extern std::shared_mutex TextObject::fontLock;
    extern std::unordered_map<std::string, sf::Font> TextObject::loadedFonts;

    TextObject::TextObject(const std::string& fontName){
        auto* font = getLoadedFont(fontName);
        std::shared_lock readLock(fontLock);
        
        if(font){
            M_currentFont = *font;
            setFont(M_currentFont);

        }
    }

    void TextObject::draw_to_screen(Scriptable::EventData* data){

        sf::Text* text = static_cast<sf::Text*>(this);
        Scipp::globalGame->window->draw(*text, sf::RenderStates::Default);
    }

    bool TextObject::loadFont(const std::string& path, const std::string& name){
        std::unique_lock writeLock(fontLock);

        if(loadedFonts.contains(name)) return false;
        if(loadedFonts[name].loadFromFile(path)){
            return true;
        }
        loadedFonts.erase(name);
        return false;
    }

    void TextObject::freeFont(const std::string& name){
        std::unique_lock writeLock(fontLock);

        if(!loadedFonts.contains(name)) return;
        loadedFonts.erase(name);
        return;
    }

    const sf::Font* TextObject::getLoadedFont(const std::string& name){
        std::shared_lock readLock(fontLock);

        if(!loadedFonts.contains(name)) return nullptr;
        return &loadedFonts.at(name);
    }
}