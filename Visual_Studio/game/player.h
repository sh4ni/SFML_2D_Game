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
	bool isAttacking;
	bool canAttack;
	char lookDirection;
    bool isInvincible;
    //float invincibleTimer;
	
	int HealTickRate;
	int pHealthMax;
	int pExp;
	int pExpMax;	// Exp needed Until Levelup
	char pGender;	// M - Male | F - Female
	std::string LevelId;

	sf::FloatRect weaponDmgBox;
	sf::Texture weaponTexture;
	sf::Sprite weaponSprite;

public:
	
	Player(int controller=0);
    
    void Init (int controller=0);
	void Update	(float ElapsedTime);
	void Render(sf::RenderWindow &Window){
        Character::Render(Window);
		if(isAttacking){
			Window.draw(weaponSprite);
		}
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
	
    void ResetCooldown(void){
        this->HealTickRate = -COOLDOWN + IDLEHEAL;
    }
    void playerDamage( int damage, int level ){
        ResetCooldown();
        if(!isInvincible){
            //isInvincible = true;
            damage = calcDamage(damage, level);
            damageText(damage,'p');
            this->Health -= damage;
            if( this->Health <= 0 ){
                this->Health = 0;
            }
        }
    }
    void playerHeal( int heal ){
        this->Health += heal;
        if( this->Health > this->pHealthMax ){
            this->Health = this->pHealthMax;
        }
    }
    void playerExp( int exp, int level ){
        if( this->Lvl < MAXLEVEL ){
            int levelDif = level-this->Lvl;
            
            float multi = 1.f;
            if(levelDif > 0){   // Gegner hat einen höhreren Level
                if(levelDif >10) levelDif = 10;
                multi += (float)levelDif*0.005f;
            }
            else if(levelDif < 0){  // Gegner hat einen niedrigeren Level
                if(levelDif < -10) levelDif = -10;
                multi += (float)levelDif*0.1f;
            }
            exp = (int)((float)exp * multi);

            /*exp += levelDif*(level/2);
            if ( exp <= 0 ){
                exp = 1;
            }*/
            damageText(exp,'e');
            this->pExp += exp;
            if( this->pExp >= this->pExpMax ){       // Hier levelup!
                this->pExp -= this->pExpMax;
                this->Lvl++;
                this->pHealthMax = FHPMAX;//(int)(BASEHEALTH*pow(HEALTHMULTIPLICATOR,(float)(this->Lvl-1)));
                this->pExpMax = FEXPMAX;//(int)(BASEEXP*pow(EXPMULTIPLICATOR,(float)(this->Lvl-1)));
                this->AttackPower = FDMG;//(int)(BASEDMG*pow(DMGMULTIPLICATOR,(float)(this->Lvl-1)));
                this->Health = this->pHealthMax;
                damageText(this->Lvl,'l');
            }
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