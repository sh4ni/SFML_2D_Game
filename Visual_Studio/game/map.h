#ifndef MAP_H_
#define MAP_H_


#include <sstream>

#include "game.h"
#include "player.h"
#include "savegame.h"
#include <vector>  



class TilePart{
public:
	sf::Sprite* TexturePart;
private:
	int TileType;
};

class Map{
public:
	void Show(sf::RenderWindow& window, std::string LevelId, sf::View viewCamera, Savegame& currentSavegame);
	void InitMap(std::string FileName);
	sf::IntRect getRect(int x, int y);
	static sf::RenderWindow& GetWindow();
	const static sf::Event& GetInput();
private:
	static bool pause(sf::RenderWindow& window, sf::View viewCamera, sf::Event levelLoop, Player& P1, std::string LevelId);
	static void resume();
	static bool save(Player& P1, std::string level);
	static bool load(Player& P1);
	std::string mapTheme;
};

#endif
