#include <StateManager.hpp>
#include <Scriptable/State.hpp>
#include <InitState.hpp>
#include <Scriptable/Entity.hpp>
#include <Camera.hpp>

#include <SFML/Graphics.hpp>

#include <iostream>

		
void InitState::onRender(const Scriptable::EventData* data)
{
    
}

void InitState::onWindowClosed(const Scriptable::EventData* data)
{
    data->targetWindow->close();
}


InitState::InitState()
{
    initCamera();
}