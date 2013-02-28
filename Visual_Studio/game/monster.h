#ifndef _MONSTER_H_
#define _MONSTER_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include "defines.h"
#include "character.h"
#include <cmath>
#include <ctime>

class Monster : public Character{
private:
	int monsterType;


public:
	Monster();
	~Monster();

	void Init();
	void Update(float ElapsedTime);
	
	void setType(int monsterType);

	bool isActive;

	sf::Texture texture;

	bool targetingPlayer;
	bool isAggressiv;

	int moveDirection;
	clock_t begin; 
	clock_t end;
};

#endif