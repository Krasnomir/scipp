
#include <Scriptable/Component.hpp>
#include <Scriptable/Components/SFMLRenderComponent.hpp>
#include <vector>
#include <SFML/Graphics.hpp>


namespace Scriptable::Components
{
    SFMLRenderComponent::SFMLRenderComponent(const std::vector<sf::Vector2f>& points) : ConvexShape(points.size()){
        for(size_t i = 0; i < points.size(); i++){
            setPoint(i, points[i]);
        }
    }

    void SFMLRenderComponent::onRender(const EventData* data){
        data->targetWindow->draw(*this);
    }

    
}

