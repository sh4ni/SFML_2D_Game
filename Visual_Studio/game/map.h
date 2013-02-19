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
	void NextLevel(sf::RenderWindow& window, std::string LevelId, sf::View viewCamera);
	int Show(sf::RenderWindow& window, std::string LevelId, sf::View viewCamera);
	void InitMap(std::string FileName);
	sf::IntRect getRect(int x, int y);
	/*static sf::RenderWindow& GetWindow();
	const static sf::Event& GetInput();*/
private:
	static bool pause(sf::RenderWindow& window, sf::View viewCamera, sf::Event levelLoop, Player& P1, std::string LevelId, sf::Clock& clock);
	static void resume();
	static void save(Player& P1, std::string level);
	static void load(Player& P1);
	std::string mapTheme;
};

#endif
