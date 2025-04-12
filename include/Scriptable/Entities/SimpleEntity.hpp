#ifndef __SIMPLE_ENTITY
#define __SIMPLE_ENTITY

#include <Scriptable/Entity.hpp>

namespace Scriptable::Entities {
    class SimpleEntity : public Scriptable::Entity {

    public:
        SimpleEntity(std::vector<sf::Vector2f> vertices);
        virtual ~SimpleEntity() = default;
    };
}

#endif
