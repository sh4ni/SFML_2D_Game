#ifndef MAP_H_
#define MAP_H_


#include <sstream>

//#include "game.h"

#include "player.h"
#include "savegame.h"
#include "schrift.h"
#include <vector>
#include "monster.h"
#include "sounds.h"

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

class MapEvent{
private:
	//
public:
	enum reason{ nothing, dead, mapchange, exiting, pause };

	MapEvent(MapEvent::reason theReason, std::string newMapId = "", float newMapPosX = 0, float newMapPosY = 0 ){
		this->theReason = theReason;
		this->newMapId = newMapId;
		this->newMapPosX = newMapPosX*TILESIZE+16;
		this->newMapPosY = newMapPosY*TILESIZE;
	};
	~MapEvent(){
	};
	
	MapEvent::reason theReason;
	std::string newMapId;
	float newMapPosX;
	float newMapPosY;
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
    
	

    void initInterface(void){
        if( P1.getGender() == 'F' ){
            if(!ifaceImage.loadFromFile(PATH"include/interface/interface-female.png")){
                throw "Error: include/interface/interface-female.png not found.";
            }
        }
        else {
            if(!ifaceImage.loadFromFile(PATH"include/interface/interface-male.png")){
                throw "Error: include/interface/interface-male.png not found.";
            }
        }
    }

	void init(std::string LevelId);
	void destory();

	MapEvent Show(sf::RenderWindow& renderWindow, std::string LevelId, sf::View viewCamera);
	
	sf::IntRect getRect(int x, int y);
	
	Player* getPlayer(void){
		return &P1;
	}

	sf::Clock* getClock(void){
		return &clock;
	}
};

#endif
