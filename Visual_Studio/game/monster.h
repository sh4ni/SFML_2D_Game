#ifndef _MONSTER_H_
#define _MONSTER_H_

#include <SFML/Graphics.hpp>
#include "character.h"

class Monster : public Character{
public:
	Monster(bool isAggresiv=false);
	~Monster();
private:
	
};

#endif