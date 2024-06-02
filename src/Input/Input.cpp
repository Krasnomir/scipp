#include <Input/Input.hpp>
#include <Game.hpp>

namespace Input{
    sf::Vector2i GetMouseScreenPosition(){
        return sf::Mouse::getPosition(*Scipp::globalGame->window);
    }
}