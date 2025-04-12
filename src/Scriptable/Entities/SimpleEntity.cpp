#include <Scriptable/Entities/SimpleEntity.hpp>

#include <Scriptable/Components/RenderComponent.hpp>

namespace Scriptable::Entities {
    SimpleEntity::SimpleEntity(std::vector<sf::Vector2f> vertices) {
        addComponent<Scriptable::Components::RenderComponent>(vertices);

        auto* rc = getComponent<Scriptable::Components::RenderComponent>();
		rc->setOrigin(rc->center());
    }
}