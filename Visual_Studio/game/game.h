#ifndef GAME_H_
#define GAME_H_

//#include "include.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "defines.h"
#include "intro.h"
#include "menu.h"
#include "map.h"
#include "savegame.h"


class Game
{
public:
	static void Init();
	static void Start(Savegame& currentSave);
	static sf::RenderWindow& GetWindow();
	const static sf::Event& GetInput();
	
private:
	static void ShowIntro();
	static void ShowMenu();
	static void GameLoop(Savegame& currentSave);
	static void ShowMap(sf::View viewCamera, Savegame& currentSave);
	static bool IsExiting();

	enum GameState { Uninitialized, Initialized, ShowingIntro, Paused, ShowingMenu, Playing, Options, Exiting };

	static GameState _gameState;
	static sf::RenderWindow _mainWindow;


	
};


#endif