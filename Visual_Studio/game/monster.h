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

	sf::Texture texture;
	sf::FloatRect hitBox;

	bool targetingPlayer;
	bool isAggressiv;

	int moveDirection;
	clock_t begin; 
	clock_t end;


public:
	Monster();
	~Monster();

	sf::FloatRect getHitBox(void){
		return this->hitBox;
	}

	void Init();
	void Update(float ElapsedTime);
	
	void setType(int monsterType);

	bool isActive;

};

#endif