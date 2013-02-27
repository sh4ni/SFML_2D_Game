#include "monster.h"

Monster::Monster(bool isAggresiv){
	Init();
};
Monster::~Monster(){

};


void Monster::setType(int monsterType){
	this->monsterType = monsterType;
}

void Monster::Init(){
	std::cout << "INIT Monster" << std::endl;
	this->Health = 200;
	this->Lvl = 1;
	this->Name = "Monster XY";
	this->PosX = 500;
	this->PosY = 600;
	this->Speed = 0.1f;
	this->isActive = true;

	sf::String tex;
	
	tex.insert(0,PATH"include/texture/monster/entity_slime_reddish.png");
	
	if(!texture.loadFromFile(tex)){
		throw "Error: Monstertexture not found.";
	}
	#ifdef DEBUGINFO
	else {
		std::cout << "Successfully loaded the Monster texture!" << std::endl;
	}
	#endif

	sprite.setTexture(texture);
	sprite.setOrigin(TILESIZE/2,TILESIZE);
	sprite.setTextureRect(sf::IntRect(0,0,TILESIZE,TILESIZE*2));
	sprite.setPosition(PosX, PosY);
}

void Monster::Update(float ElapsedTime){
	if(isActive){
		
		this->PosX = sprite.getPosition().x;
		this->PosY = sprite.getPosition().y;

		
		srand((unsigned)time(NULL));

		double myRand = (double)rand() / RAND_MAX;
		PosX += Speed*ElapsedTime;
		PosY += Speed*ElapsedTime;

		sprite.setPosition(PosX,PosY);
	}
}
