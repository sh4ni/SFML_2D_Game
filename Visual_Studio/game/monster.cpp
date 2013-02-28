#include "monster.h"
#include "map.h"
Monster::Monster(){
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
    this->Animation = 0;

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

	srand((unsigned)time(NULL));
	this->begin = clock();
	this->moveDirection = 0;
	this->targetingPlayer = false;
	this->isAggressiv = true;
}

void Monster::Update(float ElapsedTime){
	if(isActive){
		this->PosX = sprite.getPosition().x;
		this->PosY = sprite.getPosition().y;

		float PlayerPosX = Map::currentMap->getPlayer()->getPosX();
		float PlayerPosY = Map::currentMap->getPlayer()->getPosY();

		clock_t end = clock();
#ifdef SYS_MACOS
        float elapsed_secs = float(end - begin) *10/ CLOCKS_PER_SEC;
#else
		float elapsed_secs = float(end - begin) / CLOCKS_PER_SEC;
#endif
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

        bool walking = false;
		if(!targetingPlayer){
			if(elapsed_secs > HOLDTIME){
                walking = true;
				if(moveDirection == 0){
					PosY -= (Speed*ElapsedTime);
                    sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*1,TILESIZE,TILESIZE*2));
				}
                else if(moveDirection == 1){
					PosY += (Speed*ElapsedTime);
                    sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),0,TILESIZE,TILESIZE*2));
				}
                else if(moveDirection == 2){
					PosX -= (Speed*ElapsedTime);
                    sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*2,TILESIZE,TILESIZE*2));
				}
                else{
					PosX += (Speed*ElapsedTime);
                    sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*3,TILESIZE,TILESIZE*2));
				}
			}
            else {
                walking = false;
                sprite.setTextureRect(sf::IntRect(0,sprite.getTextureRect().top,TILESIZE,TILESIZE*2));  // spieler "steht", wenn er sich nicht bewegt.
            }
		}
        else{
			// wenn der Spieler entdeckt wurde
			float x = PlayerPosX - PosX;
			float y = PlayerPosY - PosY;
			if(x > MOVETOLLERANCE){
                walking = true;
				PosX += (Speed*ElapsedTime);
                sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*3,TILESIZE,TILESIZE*2));
			}else if (x < -MOVETOLLERANCE){
                walking = true;
				PosX -= (Speed*ElapsedTime);
                sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*2,TILESIZE,TILESIZE*2));
			}
			if(y > MOVETOLLERANCE){
                walking = true;
				PosY += (Speed*ElapsedTime);
                sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),0,TILESIZE,TILESIZE*2));
			}else if( y < -MOVETOLLERANCE){
                walking = true;
				PosY -= (Speed*ElapsedTime);
                sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*1,TILESIZE,TILESIZE*2));
			}

		}
        if( walking ){      // nur animieren wenn spieler lŠuft
            if( (Animation/(int)((1/Speed)*ElapsedTime*ANIMATIONSPEED)) >= 4) Animation = 0;
            Animation++;
        }

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
