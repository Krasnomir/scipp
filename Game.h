#pragma once

#include <SFML/Graphics.hpp>

#include <vector>

class StateManager;

namespace Scipp {
	extern StateManager* globalManager;
}

class Game
{
public:

	sf::Clock clock;							// liczenie deltaTime
	sf::RenderWindow *window = nullptr;			// glowne okno

	StateManager* stateManager = nullptr;		// tworzy, usuwa i zamienia stany/sceny

	void handleEvents(sf::Event event); 
	void pollEvents();							// metoda ktora przelatuje przez wszystkie aktywne eventy i przesyla je do metody "handleEvents()"
	void update(sf::Time time);
	void render();								// renderowanie elementow z "components"
	void run();									// gameLoop
	void initStates();
	void initWindow();							// Tworzenie okna
	void init();								// Odpala wszystkie inne funkcje inicjalizuj¹ce
	Game();
	
	friend class Camera;
};