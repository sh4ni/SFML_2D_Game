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


class Game
{
public:
	static void Init();
	static void Start(Savegame& currentSave, bool newgame = false);
	static sf::RenderWindow& GetWindow();
	const static sf::Event& GetInput();
	
private:
	static void ShowIntro();
	static void ShowMenu(bool newgame = false);
	static void GameLoop(Savegame& currentSave, bool newgame = false);
	static void ShowMap(sf::View viewCamera, Savegame& currentSave);
	static bool IsExiting();

	enum GameState { Uninitialized, Initialized, ShowingIntro, Paused, ShowingMenu, Playing, NewGame, Options, Exiting };

	static GameState _gameState;
	static sf::RenderWindow _mainWindow;


	
};


#endif