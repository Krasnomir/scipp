#pragma once
#include <Scriptable/UI/Object.hpp>

#ifndef _UITEXTOBJECT_HPP
#define _UITEXTOBJECT_HPP 1
namespace Scriptable::UI
{	
    class TextObject : public Object, public sf::Text{
    public:
        TextObject() = delete;

        TextObject(const std::string& fontName);
        
        virtual void draw_to_screen(Scriptable::EventData* data) override;

        ~TextObject() = default;

        static bool loadFont(const std::string& path, const std::string& name);

        static void freeFont(const std::string& name);

        static const sf::Font* getLoadedFont(const std::string& name);

    protected:

        sf::Font M_currentFont;
    private:
        static std::unordered_map<std::string, sf::Font> loadedFonts;
        static std::shared_mutex fontLock;


    };

}

#endif