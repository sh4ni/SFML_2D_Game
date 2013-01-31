#ifndef GAME_H_
#define GAME_H_

#include "include.h"

class Game
{
public:
	static void Start();
private:
	static void ShowIntro();
	static void ShowMenu();
	static void GameLoop();
	static void ShowMap(int LevelId, View view);
	static bool IsExiting();

	enum GameState { Uninitialized, ShowingIntro, Paused, ShowingMenu, Playing, Exiting };

	static GameState _gameState;
	static RenderWindow _mainWindow;
};

#endif