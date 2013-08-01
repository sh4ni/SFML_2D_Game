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
    
    int maxHealth;

	sf::Texture texture;
	sf::FloatRect hitBox;

#ifdef DEBUGINFO
    sf::RectangleShape drawHitBox;
#endif
    
    sf::RectangleShape hpBox;
    Schrift nameBox;
    Schrift levelBox;
    
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

    /// Gibt die Hitbox des Monsters zurück. Sie bestimmt, wo das Monster angegriffen werden kann.
	sf::FloatRect getHitBox(void){
		return this->hitBox;
	}

	void Init();
	void Update(float ElapsedTime);
	
	void setType(int monsterType);
    
    /// Gibt die Art des Monsters zurück.
    int getType(void){
        return this->monsterType;
    }
    
    void damageMe( int damage, int level );
    
    /// Lässt das Monster den Spieler angreifen.
    void targetPlayer(){
        targetingPlayer = true;
    }
    
    /// Zeichnet das Monster
    void Render(sf::RenderWindow &renderWindow){
        if(this->isActive){
            if(this->targetingPlayer){
                renderWindow.draw(hpBox);
                nameBox.Render(renderWindow);
                levelBox.Render(renderWindow);
            }
            Character::Render(renderWindow, isActive);
#ifdef DEBUGINFO
            renderWindow.draw(drawHitBox);
#endif
        }
    }
};

#endif