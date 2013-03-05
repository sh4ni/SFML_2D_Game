#ifndef GAME_H_
#define GAME_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include "defines.h"
#include "intro.h"
#include "menu.h"
#include "map.h"
#include "savegame.h"
#include "pause.h"
#include "sounds.h"

class Game{
private:
	enum GameState { Uninitialized, Initialized, ShowingIntro, Paused, ShowingMenu, Playing, Continue, NewGame, Options, Exiting, ShowingGenderMenu };
	static GameState _gameState;
	static sf::RenderWindow _mainWindow;	

	Game(void){
		// Konstruktor
	};
	~Game(void){
		// Destruktor
	};

	static void ShowIntro();
	static void ShowMenu();
    static const char ShowMenuGender();
    static void ShowMenuOptions();

	static void GameLoop();
	static void GamePaused(sf::View viewCamera);

	static void ShowMap(sf::View viewCamera);

	static bool IsExiting();
public:
	static void Init();
	static void Start();
	static bool isNewGame;
	static bool continueGame;
};
#endif