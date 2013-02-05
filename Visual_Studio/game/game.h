#ifndef GAME_H_
#define GAME_H_

#include "include.h"

class Game
{
public:
	static void Start();
	static sf::RenderWindow& GetWindow();
	const static sf::Event& GetInput();
private:
	static void ShowIntro();
	static void ShowMenu();
	static void GameLoop();
	static void ShowMap(int LevelId, View viewCamera);
	static bool IsExiting();

	enum GameState { Uninitialized, ShowingIntro, Paused, ShowingMenu, Playing, Options, Exiting };

	static GameState _gameState;
	static RenderWindow _mainWindow;
};

#endif