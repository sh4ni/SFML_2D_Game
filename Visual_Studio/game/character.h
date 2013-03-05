#ifndef _CHARACTER_H_
#define _CHARACTER_H_

#include <SFML/Graphics.hpp>
#include <sstream>
#include "schrift.h"

class Character{
protected:
    sf::Texture texture;
	sf::Sprite sprite;
	sf::IntRect*** ColMap;
	sf::Vector2i MapSize;
	int Animation;

	float PosX;
	float PosY;
	float Speed;
	
	std::string Name;
    
    std::vector<Schrift> dmgText;

	int Health;
	int AttackPower;
	int Lvl;

	// not in use yet
	//int strenght;
	//int agility;
	//int vitality;

public:
	Character();
	~Character();

	// getter and settter
	float getPosX(void);
	float getPosY(void);
	void setPosition( float X, float Y);
	void setMapSize( int xMax, int yMax);

	float getSpeed(void);
	void increaseSpeed(float speedValue);
	void decreaseSpeed(float speedValue);

	void setName(std::string Name);
	std::string getName(void);

	int getHealth(void);
	void setHealth(int Health);

	void setLvl(int Lvl);
	int getLvl(void);

	void setColMap(sf::IntRect***& ColMap);
    
    int calcDamage(int damage,int level);
    
    void damageText(int damage, char type);
    
    void Render(sf::RenderWindow &renderWindow, bool drawEnemy = true);

};

#endif