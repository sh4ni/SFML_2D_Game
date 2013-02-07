#ifndef MAP_H_
#define MAP_H_

#include "include.h"

using namespace sf;

class TilePart{
public:
	Sprite* TexturePart;
private:
	int TileType;
};

class Map{
public:
	void Show(RenderWindow& window, int LevelId, View viewCamera);
	void InitMap(std::string FileName);
	IntRect getRect(int x, int y);
private:
	static bool pause(RenderWindow& window, View viewCamera, Event levelLoop, bool paused);
	static void resume();
};

#endif
