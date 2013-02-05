#ifndef MAP_H_
#define MAP_H_

#include "include.h"

using namespace sf;

class TilePart{
public:
	void LoadTile(Texture Texture);
	void DrawTile(RenderWindow &window);
	void SetTileType(int TileNumber);
	Sprite* TexturePart;
private:
	int TileType;
};

class Map{
public:
	void Show(RenderWindow& window, int LevelId, View viewCamera);
	void InitMap(std::string FileName);
private:
	static void pause(RenderWindow& window, View viewCamera, Event levelLoop, bool paused);
	static void resume();
};

#endif
