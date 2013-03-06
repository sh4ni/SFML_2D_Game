#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include "defines.h"
#include "character.h"


class Player : public Character{
private:
    int controller;
	bool blockControl;
	bool isAttacking;
	bool canAttack;
	char lookDirection;
    bool isInvincible;
    bool showSword;
    float swordTimer;
	
	int HealTickRate;
	int pHealthMax;
	int pExp;
	int pExpMax;	// Exp needed Until Levelup
	char pGender;	// M - Male | F - Female
	std::string LevelId;

	sf::FloatRect weaponDmgBox;
	sf::Texture weaponTexture;
	sf::Sprite weaponSprite;
    
#ifdef DEBUGINFO
    sf::RectangleShape drawSwordBox;
#endif

public:
	
	Player(int controller=0);
    
    void Init (int controller=0);
	void Update	(float ElapsedTime);
	void Render(sf::RenderWindow &Window);
	
	std::string getLevelId(void){
		return this->LevelId;
	}

	void setLevelId(std::string LevelId){
		this->LevelId = LevelId;
	}
	
	int getHealth(void){
		return this->Health;
	}
	
	void setExp(int pExp){
		this->pExp = pExp;
	}
	int getExp(void){
		return this->pExp;
	}
	char getGender(void){
		return this->pGender;
	}
	void setGender(char pGender){
		this->pGender = pGender;
	};
	int getHealthMax(void){
		return this->pHealthMax;
	}
	int getExpMax(void){
		return this->pExpMax;
	}
    void setBlockControl(bool block=false){
        this->blockControl = block;
    }
	
    void ResetCooldown(void);
    void playerDamage( int damage, int level );
    void playerHeal( int heal );
    void playerExp( int exp, int level );
};

#endif