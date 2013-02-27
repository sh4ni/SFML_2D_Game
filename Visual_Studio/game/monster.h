#ifndef _MONSTER_H_
#define _MONSTER_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include "defines.h"
#include "character.h"

#include <ctime>

class Monster : public Character{
private:
	int monsterType;


public:
	Monster(bool isAggresiv=true);
	~Monster();

	void Init();
	void Update(float ElapsedTime);
	
	void setType(int monsterType);

	bool isActive;


	bool targetingPlayer;

	int moveDirection;
	clock_t begin; 
	clock_t end;
};

#endif