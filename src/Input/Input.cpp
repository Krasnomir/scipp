#include <Input/Input.hpp>
#include <Game.h>

namespace Input{
    sf::Vector2i GetMouseScreenPosition(){
        return sf::Mouse::getPosition(*Scipp::globalGame->window);
    }
}