#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <math.h>
#include "defines.h"
#include "character.h"


class Player : public Character{
private:
    int controller;
	bool blockControl;
	
	int HealTickRate;
	int pHealthMax;
	int pExp;
	int pExpMax;	// Exp needed Until Levelup
	char pGender;	// M - Male | F - Female
	std::string LevelId;
	
public:
	
	Player(int controller=0);

	void Update	(float ElapsedTime);
	void Render	(sf::RenderWindow &Window){
		Window.draw(sprite);
	}
	
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
	void setMapSize( int xMax, int yMax){
		this->MapSize.x = xMax;
		this->MapSize.y = yMax;
	}
    void ResetCooldown(void){
        this->HealTickRate = -COOLDOWN + IDLEHEAL;
    }
    void playerDamage( int damage, int level ){
        ResetCooldown();
		int levelDif = level-this->Lvl;
        damage += (levelDif*(level/10));
        if ( damage <= 0 ){
            damage = 1;
        }
        this->Health -= damage;
        if( this->Health < 0 ){
            this->Health = 0;
			std::cout << "Player will be die!" << std::endl;
        }
    }
    void playerHeal( int heal ){
        this->Health += heal;
        if( this->Health > this->pHealthMax ){
            this->Health = this->pHealthMax;
        }
    }
    void playerExp( int exp, int level ){
        int levelDif = level-this->Lvl;
        exp += levelDif*(level/2);
        this->pExp += exp;
        if( this->pExp >= this->pExpMax ){       // Hier levelup!
            this->pExp -= this->pExpMax;
            this->Lvl++;
            this->pHealthMax = (int)(BASEHEALTH*pow(HEALTHMULTIPLICATOR,(float)(this->Lvl-1)));
            this->pExpMax = (int)(BASEEXP*pow(EXPMULTIPLICATOR,(float)(this->Lvl-1)));
			this->Health = this->pHealthMax;
            #ifdef DEBUGINFO
                std::cout << "LEVEL UP: " << this->Lvl << std::endl;
            #endif
        }
    }
	void setBlockControl(bool block=false){
		this->blockControl = block;
		#ifdef DEBUGINFO
			std::cout << "block " << block << std::endl;
		#endif
	}

};

#endif