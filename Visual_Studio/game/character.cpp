#include "character.h"

Character::Character(){

};
Character::~Character(){

};

void Character::Update(float ElapsedTime){

}

void Character::Render(sf::RenderWindow &Window){
	Window.draw(sprite);
}

float Character::getPosX(void){
	return this->PosX;
}
float Character::getPosY(void){
	return this->PosY;
}
void Character::setPosition(float x, float y){
    this->PosX = x;
    this->PosY = y;
	sprite.setPosition(x,y);
}

float Character::getSpeed(void){
	return this->Speed;
}
void Character::increaseSpeed(float speedValue){
	if(this->Speed < 0.35f)
		this->Speed += speedValue;
}
void Character::decreaseSpeed(float speedValue){
	if(this->Speed <= 0.2f)
		this->Speed = 0.1f;
	else
		this->Speed -= speedValue;
}

void Character::setName(std::string Name){
	this->Name = Name;
}
std::string Character::getName(void){
	return this->Name;
}


int Character::getHealth(void){
	return this->Health;
}
void Character::setHealth(int Health){
	this->Health = Health;
}

void Character::setLvl(int Lvl){
	this->Lvl = Lvl;
}
int Character::getLvl(void){
	return this->Lvl;
}

void Character::setColMap(sf::IntRect*** ColMap){
	this->ColMap = ColMap;
};

/*
void Character::setBlockControl(bool block){
	this->blockControl = block;
	#ifdef DEBUGINFO
		std::cout << "block " << block << std::endl;
	#endif
}
*/