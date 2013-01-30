#ifndef MAP_H_
#define MAP_H_

#include "include.h"

using namespace sf;

#define TILESIZE 32

/*class TextureLoader{
public:
	void LoadTexture(std::string TextureFile);
private:
};*/

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
	void Show(RenderWindow& window, int LevelId);
	void InitMap(std::string FileName);

private:

};

#endif
