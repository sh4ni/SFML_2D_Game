#ifndef _MONSTER_H_
#define _MONSTER_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include "defines.h"
#include "character.h"
#include <math.h>
#include <ctime>

class Monster : public Character{
private:
	int monsterType;
    int exp;
	bool isActive;

	sf::Texture texture;
	sf::FloatRect hitBox;

	bool targetingPlayer;
	bool isAggressiv;
    bool isHit;

	int moveDirection;
	clock_t begin; 
	clock_t end;

    float hitTimer;

public:
	Monster();
	~Monster();

	sf::FloatRect getHitBox(void){
		return this->hitBox;
	}

	void Init();
	void Update(float ElapsedTime);
	
	void setType(int monsterType);
    
    void damageMe( int damage, int level );
    
    void targetPlayer(){
        targetingPlayer = true;
    }
    
    void Render(sf::RenderWindow &Window){
        Character::Render(Window, isActive);
    }
};

#endif