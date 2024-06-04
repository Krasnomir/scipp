#include <StateManager.h>
#include <Scriptable/State.hpp>
#include <InitState.h>
#include <Scriptable/Entity.hpp>

#include <SFML/Graphics.hpp>

#include <iostream>

#include <Camera.hpp>

		
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