#ifndef GAME_H_
#define GAME_H_

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>

#include "defines.h"
#include "player.h"
#include "npc.h"
#include "schrift.h"
#include "map.h"
#include "splashScreen.h"

using namespace sf;

class Game
{
public:
	static void Start();
private:
	static bool IsExiting();
	static void GameLoop();

	static void ShowSplashScreen();

	enum GameState { Uninitialized, ShowingSplash, Paused, ShowingMenu, Playing, Exiting };
  
	static GameState _gameState;
	static sf::RenderWindow _mainWindow;
};

#endif