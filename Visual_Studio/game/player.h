#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include "defines.h"
#include "savegame.h"


class Player{
public:
	void Update		(sf::RenderWindow &Window, float ElapsedTime);
	void Render		(sf::RenderWindow &Window){
		Window.draw(sprite);
	}
	Player			(sf::IntRect*** CollisionMap, Savegame& currentSavegame);
	float			getPosX(void){
		return this->x;
	}
	float			getPosY(void){
		return this->y;
	}
	void			setPosition(float PosX, float PosY){
		sprite.setPosition(PosX,PosY);
	}
	float			getSpeed(void){
		return this->Speed;
	}
	void			increaseSpeed(float speedValue){
		if(this->Speed < 0.35f)
			this->Speed += speedValue;
	}
	void			decreaseSpeed(float speedValue){
		if(this->Speed <= 0.2f)
			this->Speed = 0.1f;
		else
			this->Speed -= speedValue;
	}
	void setHealth(int pHealth){
		this->pHealth = pHealth;
	}
	void setLvl(int pLvl){
		this->pLvl = pLvl;
	}
	void setExp(int pExp){
		this->pExp = pExp;
	}
	void setName(std::string pName){
		this->pName = pName;
	}
	int getHealth(void){
		return this->pHealth;
	}
	int getLvl(void){
		return this->pLvl;
	}
	int getExp(void){
		return this->pExp;
	}
	char getGender(void){
		return this->pGender;
	}
	std::string getName(void){
		return this->pName;
	}
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
private:
	float Speed;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::IntRect*** ColMap;
	sf::Vector2i MapSize;
	float x;
	float y;
	unsigned char Animation;

	int pHealth;
	int pHealthMax;
	int pLvl;
	int pExp;
	int pExpMax;	// Exp needed Until Levelup
	char pGender;	// M - Male | F - Female
	std::string pName; 

};

#endif