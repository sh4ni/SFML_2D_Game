#ifndef MAP_H_
#define MAP_H_


#include <sstream>

//#include "game.h"

#include "player.h"
#include "savegame.h"
#include "schrift.h"
#include <vector>


#include "monster.h"


//class Player;

class TilePart{
public:
	sf::Sprite* TexturePart;
private:
	int TileType;
};

class Map{
public:
	Map();
	~Map();
	
	static Map * currentMap;

	int Show(sf::RenderWindow& window, std::string LevelId, sf::View viewCamera);
	
	sf::IntRect getRect(int x, int y);
	/*static sf::RenderWindow& GetWindow();
	const static sf::Event& GetInput();*/
private:
//	static bool pause(sf::RenderWindow& window, sf::View viewCamera, sf::Event levelLoop, Player& P1, std::string LevelId, sf::Clock& clock);
	static void resume();
	
	static void save(Player& P1, std::string level);
	static void load(Player& P1);

	std::string mapTheme;

	////////////// das ist toll / danner 

	static sf::Texture LevelTexture;

	int MapSizeX;
	int MapSizeY;
	int LoadCounterX;
	int LoadCounterY;
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
	
	Player* getPlayer(void){
		return &P1;
	}
};

#endif
