#ifndef MAP_H_
#define MAP_H_


#include <sstream>

//#include "game.h"

#include "player.h"
#include "savegame.h"
#include "schrift.h"
#include <vector>
#include "monster.h"

class tp{
	public:
		std::string Map;
		int xDest;
		int yDest;
};

class TilePart{
private:
	int TileType;
public:
	sf::Sprite* TexturePart;
	int EnemyId;
	tp* Teleport;
};

class Map{

private:

	std::string mapTheme;


	static sf::Texture LevelTexture;

	int MapSizeX;
	int MapSizeY;
	int MapLevelMin;
	int MapLevelMax;
	int TileType;
	TilePart** TileMap;
	sf::IntRect*** CollisionMap;

	std::string FileName;
	
	sf::Clock clock;

	Player P1;
	
	float LastTime;
	float ElapsedTime;
	float Frames;

	int CamX;
	int CamY;

	int TileX;
	int TileY;

	sf::Texture ifaceImage;         // lade interface entsprechend des gewählten heldens.
	sf::Sprite iface;

	sf::RectangleShape HPBar;
	sf::RectangleShape EXPBar;

	Schrift DisplayHPText;
	Schrift DisplayEXPText;
	Schrift DisplayHP;   
	Schrift DisplayEXP;
	Schrift DisplayLevel;

	Schrift DisplayFPS;
	Schrift DisplayKoord;
	Schrift DisplaySpeed;

public:
	Map();
	~Map();
	
	static Map * currentMap;

	int Show(sf::RenderWindow& window, std::string LevelId, sf::View viewCamera);
	
	sf::IntRect getRect(int x, int y);
	
	Player* getPlayer(void){
		return &P1;
	}

	sf::Clock* getClock(void){
		return &clock;
	}
};

#endif
