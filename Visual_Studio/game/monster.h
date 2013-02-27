#ifndef _MONSTER_H_
#define _MONSTER_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include "defines.h"
#include "character.h"

class Monster : public Character{
private:
	int monsterType;
public:
	Monster(bool isAggresiv=false);
	~Monster();

	void Init();
	void Update(float ElapsedTime);
	
	void setType(int monsterType);

	bool isActive;
};

#endif