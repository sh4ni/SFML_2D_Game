#ifndef _MONSTER_H_
#define _MONSTER_H_

#include <SFML/Graphics.hpp>
#include <iostream>
#include "defines.h"
#include "character.h"
#include <math.h>
#include <ctime>

/// Die Klasse für die Monster, sie ist abgeleitet vom Charakter.
class Monster : public Character{
private:
	int monsterType;
    int exp;
	bool isActive;

	sf::Texture texture;
	sf::FloatRect hitBox;

#ifdef DEBUGINFO
    sf::RectangleShape drawHitBox;
#endif
    
	bool targetingPlayer;
	bool isAggressiv;
    bool isHit;
    
    char isSpecial;
    bool isBig;

	int moveDirection;
	clock_t begin; 
	clock_t end;

    float hitTimer;
    float dmgTimer;
    bool canAttack;

public:
	Monster();
	~Monster();

	sf::FloatRect getHitBox(void){
		return this->hitBox;
	}

	void Init();
	void Update(float ElapsedTime);
	
	void setType(int monsterType);
    
    int getType(void){
        return this->monsterType;
    }
    
    void damageMe( int damage, int level );
    
    void targetPlayer(){
        targetingPlayer = true;
    }
    
    void Render(sf::RenderWindow &renderWindow){
        Character::Render(renderWindow, isActive);
#ifdef DEBUGINFO
        if(this->isActive)renderWindow.draw(drawHitBox);
#endif
    }
};

#endif