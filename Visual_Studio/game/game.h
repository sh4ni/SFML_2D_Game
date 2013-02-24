#ifndef GAME_H_
#define GAME_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include "defines.h"
#include "intro.h"
#include "menu.h"
#include "map.h"
#include "savegame.h"
#include "pause.h"


class Game{
public:
	static void Init();
	static void Start(bool newgame = false);
	/*static Savegame currentSaveGame;
	static ConfigFile currentConfigFile;*/
	
private:
	enum GameState { Uninitialized, Initialized, ShowingIntro, Paused, ShowingMenu, Playing, Continue, NewGame, Options, Exiting, ShowingGenderMenu };
	static void ShowIntro();
	static void GamePaused(sf::View viewCamera);
	static void ShowMenu(bool newgame = false);
    static const char ShowMenuGender();
    static void ShowMenuOptions();
	static void GameLoop(bool newgame = false);
	static void ShowMap(sf::View viewCamera, bool continueGame = false);
	static bool IsExiting();
	static GameState _gameState;
	static sf::RenderWindow _mainWindow;
};


#endif