#ifndef MAP_H_
#define MAP_H_


#include <sstream>

#include "game.h"
#include "player.h"
#include "savegame.h"
#include <vector>  



class TilePart{
public:
	Sprite* TexturePart;
private:
	int TileType;
};

class Map{
public:
	void Show(sf::RenderWindow& window, int LevelId, sf::View viewCamera, Savegame& currentSavegame);
	void InitMap(std::string FileName);
	sf::IntRect getRect(int x, int y);
private:
	static bool pause(sf::RenderWindow& window, View viewCamera, Event levelLoop, bool paused, Player& P1, int LevelId, Savegame& currentSavegame);
	static void resume();
	static bool save(Player& P1, int level);
	static bool load(Player& P1);
};

#endif
