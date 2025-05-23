#include <StateManager.hpp>
#include <Scriptable/State.hpp>
#include <InitState.hpp>
#include <GameState.hpp>
#include <DeathState.hpp>
#include <Scriptable/Entity.hpp>
#include <Camera.hpp>
#include <Scriptable/Components/RenderComponent.hpp>
#include <Util.hpp>
#include <Game.hpp>

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
    // initCamera();
}

void InitState::init()
{
    Scipp::globalGame->stateManager.changeState(new GameState());
    Scipp::globalGame->stateManager.currentState->init();
}