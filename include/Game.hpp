#pragma once

#include <StateManager.hpp>

#include <SFML/Graphics.hpp>

#include <vector>

class StateManager;

class Game
{
public:
	sf::Time deltaTime;

	sf::RenderWindow *window = nullptr;			// glowne okno

	StateManager stateManager;					// tworzy, usuwa i zamienia stany/sceny

	void init();								// Odpala wszystkie inne funkcje inicjalizuj�ce

	void pollEvents();							// metoda ktora przelatuje przez wszystkie aktywne eventy i przesyla je do metody "handleEvent()"

	void run();									// gameLoop
	void initStates();
	void initWindow();							// Tworzenie okna

	Game();
	
	friend class Camera;
private:
	void handleEvent(sf::Event event); 
	sf::Clock M_clock;							// liczenie deltaTime
	Scriptable::EventData M_eventData; 
};

namespace Scipp {
	extern Game* globalGame;
}