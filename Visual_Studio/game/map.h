#ifndef MAP_H_
#define MAP_H_

#include <sstream>
#include "player.h"
#include "savegame.h"
#include "schrift.h"
#include <vector>
#include "monster.h"
#include "sounds.h"
#include <ctime>

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
		this->newMapPosX = newMapPosX*TILESIZE+TILESIZE/2;
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
	std::string mapMusic;

    std::string nextMap[4]; // 0 = oben // 1 = unten // 2 = links // 3 = rechts
    
	static sf::Texture LevelTexture;

	int MapSizeX;
	int MapSizeY;
	int MapLevelMin;
	int MapLevelMax;
	int TileType;
	TilePart** TileMap;
	sf::IntRect*** CollisionMap;
	sf::Clock clock;

	Player P1;

	//std::vector<Monster> monsterList;
	Monster* monsterList;
	int monsterCounter;
	
	float LastTime;
	float ElapsedTime;
	float Frames;

	bool isZoom;
    bool willPause;

	int CamX;
	int CamY;

	int TileX;
	int TileY;

	sf::Texture ifaceImage;         // lade interface entsprechend des gewŠhlten heldens.
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
    
    sf::Vector2i getMonsterLevel(void){
        return sf::Vector2i(this->MapLevelMin,this->MapLevelMax);
    }

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

	sf::Vector2i getNextLevelSize(std::string NevtLevel){
		std::string FileName = PATH"include/map/" + NevtLevel + ".txt";
		int x = 0;
		int y = 0;
		std::ifstream openfile(FileName.c_str());
		if( openfile.is_open() ){
			openfile >> x >> y;
			std::ifstream closefile(FileName.c_str());
		}
		else {
			throw "Error: " + FileName + " not found.";
		}
		return sf::Vector2i(x,y);
	}

	void init(std::string LevelId);
	void destory();

	MapEvent Show(sf::RenderWindow& renderWindow, std::string LevelId, sf::View& viewCamera);
	
	sf::IntRect getRect(int x, int y);
	
	Player* getPlayer(void){
		return &P1;
	}

	int getMonsterCounter(){
		return this->monsterCounter;
	}

	Monster* getMonsterList(){
		return this->monsterList;
	}

	sf::Clock* getClock(void){
		return &clock;
	}

};

#endif
