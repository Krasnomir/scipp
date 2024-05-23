#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>



#include <vector>

#include <Camera.hpp>
#include <Scriptable/Entity.hpp>

class StateManager;

class State : public Scriptable::EventObject
{
public:
	State();
	virtual void render();
	virtual void update(sf::Time elapsed);
	virtual void init();

	// TODO: void addEntity(const std::string& entityName, const Scriptable::Entity& entity); 
	// TODO: void deleteEntity(const std::string& entityName);	

private:

	//every entity has a name (like in unity), only for internal use, please refer to the get/set methods for external use
	std::unordered_map<std::string, Scriptable::Entity*> M_entityMap;

	Camera M_RenderCamera;
	
};