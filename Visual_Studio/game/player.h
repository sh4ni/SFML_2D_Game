#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <cmath>
#include "defines.h"
#include "character.h"

/// Die Klasse für den Spieler. Sie wird ebenfalls vom Charakter abgeleitet.
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
	
    /// Im Player wird die Map gespeichert, auf der er sich befindet. Dies ist wichtig für das Speichern und Laden des Spielstandes.
    /// Setzt die Map. (für den Spielstand)
	std::string getLevelId(void){
		return this->LevelId;
	}

    /// Gibt die Map zurück. (für den Spielstand)
	void setLevelId(std::string LevelId){
		this->LevelId = LevelId;
	}
	
    /// Gibt die aktuellen Lebenspunkte des Spielers zurück. (für den Spielstand)
	int getHealth(void){
		return this->Health;
	}
	
    /// Setzt die aktuelle Erfahrung des Spielers auf den übergebenen Wert. (für den Spielstand)
	void setExp(int pExp){
		this->pExp = pExp;
	}
    
    /// Gibt die aktuelle Erfahrung des Spielers zurück. (für den Spielstand)
	int getExp(void){
		return this->pExp;
	}
    
    /// Gibt den vom Spieler ausgewählten Helden zurück. (für den Spielstand)
	char getGender(void){
		return this->pGender;
	}
    
    /// Setzt die Spielfigur auf den übergebenen Helden. (für den Spielstand)
	void setGender(char pGender){
		this->pGender = pGender;
	};
    
    /// Gibt das maximale Leben zurück, welches der Spieler fassen kann. (für das Interface)
	int getHealthMax(void){
		return this->pHealthMax;
	}
    
    /// Gibt die maximalen Erfahrungspunkte zurück, welcher der Spieler fassen kann. (für das Interface)
	int getExpMax(void){
		return this->pExpMax;
	}
    
    /// Die Steuerung des Spielers wird blockiert oder wieder freigegeben.
    void setBlockControl(bool block=false){
        this->blockControl = block;
    }
	
    void ResetCooldown(void);
    void playerDamage( int damage, int level );
    void playerHeal( int heal );
    void playerExp( int exp, int level );
};

#endif