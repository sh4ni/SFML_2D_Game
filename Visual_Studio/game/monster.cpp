#include "monster.h"
#include "map.h"
Monster::Monster(){
	
};
Monster::~Monster(){

};


void Monster::setType(int monsterType){
	this->monsterType = monsterType;
	Init();
}

void Monster::Init(){
	std::cout << "INIT Monster" << std::endl;
	this->Health = 200;
	this->Lvl = 1;
	this->Name = "Monster XY";
	this->Speed = 0.1f;
	this->isActive = true;
	
	std::cout << monsterType << std::endl;
	if( monsterType == 1 ){
		this->isAggressiv = true;
		this->tex.insert(0,PATH"include/texture/monster/entity_slime_reddish.png");
	}else if( monsterType == 2){
		this->isAggressiv = true;
		this->tex.insert(0,PATH"include/texture/monster/entity_slime_blue.png");
	}else if( monsterType == 3){
		this->isAggressiv = true;
		this->tex.insert(0,PATH"include/texture/monster/entity_slime_green.png");
	}

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
	srand((unsigned)time(NULL));
	this->begin = clock();
	this->moveDirection = 0;
}

void Monster::Update(float ElapsedTime){
	if(isActive){
		this->PosX = sprite.getPosition().x;
		this->PosY = sprite.getPosition().y;

		float PlayerPosX = Map::currentMap->getPlayer()->getPosX();
		float PlayerPosY = Map::currentMap->getPlayer()->getPosY();

		clock_t end = clock();
		float elapsed_secs = float(end - begin) / CLOCKS_PER_SEC;
		//std::cout << elapsed_secs << std::endl;
		if( elapsed_secs > (HOLDTIME + MOVETIME) ){
			this->begin = end;
			this->moveDirection = (int)rand() % 4;	// 0 up - 1 down - 2 left - 3 right
		}
			
		if(isAggressiv || targetingPlayer){
			float x = PlayerPosX - PosX;
			float y = PlayerPosY - PosY;

			if(x < 0) x *= -1;
			if(y < 0) y *= -1;

			x = sqrt(pow(x,2)+pow(y,2));

			if ( x < (float)(DETECTIONRADIUS*TILESIZE) ){
				this->targetingPlayer = true;
			}else if( x > LOSTRADIUS){
				this->targetingPlayer = false;
			}
		}

		if(!targetingPlayer){
			if(elapsed_secs > HOLDTIME){
				if(moveDirection == 0){
					PosY -= (Speed*ElapsedTime);
				}else if(moveDirection == 1){
					PosY += (Speed*ElapsedTime);
				}else if(moveDirection == 2){
					PosX -= (Speed*ElapsedTime);
				}else{
					PosX += (Speed*ElapsedTime);
				}
			}
		}else{
			// wenn der Spieler entdeckt wurde
			float x = PlayerPosX - PosX;
			float y = PlayerPosY - PosY;
			
			if(x > 0){
				PosX += (Speed*ElapsedTime);
			}else{
				PosX -= (Speed*ElapsedTime);
			}
			if(y > 0){
				PosY += (Speed*ElapsedTime);
			}else{
				PosY -= (Speed*ElapsedTime);
			}

		}




		/*
		 *
		 *
		MONSTER LÄUFT RANDOM RUM
		 *
		 *
		 */

		;;;;;

		/*
		 *
		 *
		KOLLISIONSABFRAGE DES MONSTERS
		 *
		 *
		 */

		sprite.setPosition(PosX,PosY);
	}
}
