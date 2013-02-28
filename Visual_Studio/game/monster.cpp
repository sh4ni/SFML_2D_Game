#include "monster.h"
#include "map.h"
Monster::Monster(){
	this->Speed = 0.1f;
	this->isActive = true;
    this->Animation = 0;
};
Monster::~Monster(){

};


void Monster::setType(int monsterType){
	this->monsterType = monsterType;
	Init();
}

void Monster::Init(){
	std::cout << "INIT Monster" << std::endl;
    
    int minLevel = Map::currentMap->getMonsterLevel().x;
    int maxLevel = Map::currentMap->getMonsterLevel().y;
    
    this->Lvl = (rand() % maxLevel)+minLevel;
    
	sf::String tex;
	//std::cout << monsterType << std::endl;
	this->Health = (int)(MONSTERBASEHEALTH*pow(HEALTHMULTIPLICATOR,(float)(this->Lvl-1)));
    this->isAggressiv = true;
    switch ( monsterType ){
        case 1:
            this->Name = "Green Slime";
            tex.insert(0,PATH"include/texture/monster/entity_slime_green.png");
            this->isAggressiv = false;
            break;
        case 2:
            this->Name = "Red Slime";
            tex.insert(0,PATH"include/texture/monster/entity_slime_reddish.png");
            break;
        case 3:
            this->Name = "Blue Slime";
            tex.insert(0,PATH"include/texture/monster/entity_slime_blue.png");
            this->isAggressiv = false;
            break;
        default:
            break;
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
	this->begin = clock();
    this->moveDirection = (int)rand() % 4;	// 0 up - 1 down - 2 left - 3 right
}

void Monster::Update(float ElapsedTime){
	if(isActive){
		this->PosX = sprite.getPosition().x;
		this->PosY = sprite.getPosition().y;
        
        int tx = ((int)PosX/TILESIZE)-1;
        int ty = ((int)PosY/TILESIZE)-1;

        // Monsterkollision
        
        bool blockUp = false;
        bool blockDown = false;
        bool blockLeft = false;
        bool blockRight = false;
        
        // 12 Kollisionspunkte
        int CollisionPoint[12][2] = {
            {(int)PosX-TILESIZE/2, (int)PosY},										//  0: Links Oben						+--- Player ----+
            {(int)PosX+TILESIZE/2-1, (int)PosY},									//  1: Rechts Oben						|				|
            {(int)PosX-TILESIZE/2, (int)PosY+TILESIZE-1},							//  2: Links Unten						|				|
            {(int)PosX+TILESIZE/2-1, (int)PosY+TILESIZE-1},							//  3: Rechts Unten						|				| keine Kollisionsabfrage
            {(int)PosX-TILESIZE/2+COLLISIONTOLERANCE, (int)PosY},					//  4: Links Oben Hilfspunkt Oben		|				| im oberen bereich.
            {(int)PosX+TILESIZE/2-1-COLLISIONTOLERANCE, (int)PosY},					//  5: Rechts Oben Hilfspunkt Oben		|				|
            {(int)PosX-TILESIZE/2+COLLISIONTOLERANCE, (int)PosY+TILESIZE-1},		//  6: Links Unten Hilfspunkt Unten		| 00 04   05 01 |
            {(int)PosX+TILESIZE/2-1-COLLISIONTOLERANCE, (int)PosY+TILESIZE-1},		//  7: Rechts Unten Hilfspunkt Unten	| 08         09 |
            {(int)PosX-TILESIZE/2, (int)PosY+COLLISIONTOLERANCE},					//  8: Links Oben Hilfspunkt Links		|				|
            {(int)PosX+TILESIZE/2-1, (int)PosY+COLLISIONTOLERANCE},					//  9: Rechts Oben Hilfspunkt Rechts	| 10         11 |
            {(int)PosX-TILESIZE/2, (int)PosY+TILESIZE-1-COLLISIONTOLERANCE},		// 10: Links Unten Hilfspunkt Links		| 02 06   07 03 |
            {(int)PosX+TILESIZE/2-1, (int)PosY+TILESIZE-1-COLLISIONTOLERANCE},		// 11: Rechts Unten Hilfspunkt Rechts	+---------------+
            
        };
        
        bool cp[12] = {0,0,0,0,0,0,0,0,0,0,0,0};    // alle kollisionspunkte sind erstmal false
        
        for( int i=0; i<9; i++){
            if(tx+(i%3) >= 0 && ty+i/3 >= 0 && tx+(i%3) < MapSize.x && ty+i/3 < MapSize.y && ColMap[tx+(i%3)][ty+i/3] != NULL){
                for( int p=0; p<12; p++){
                    if( ColMap[tx+(i%3)][ty+i/3]->contains(CollisionPoint[p][0],CollisionPoint[p][1]) ){
                        cp[p] = 1;
                    }
                }       // setze alle kollisionspunkte auf 1, welche eine kollision mit der welt haben.
            }
        }
        
        for( int i=0; i<12; i++){	// Punktabfrage sollte jetzt passen. Fall zu untersuchen: 0 !4 !8 und dann gegen die ecke laufen...
            if( ((cp[0] && cp[4]) || (cp[1] && cp[5])) && (!cp[8] || !cp[9]) ){ // Bei Normaler Auslastung
                blockUp = true;
            }
            else if( ((cp[2] && cp[6]) || (cp[3] && cp[7])) && (!cp[10] || !cp[11]) ){
                blockDown = true;
            }
            if( ((cp[0] && cp[8]) || (cp[2] && cp[10])) && (!cp[4] || !cp[6]) ){
                blockLeft = true;
            }
            else if( ((cp[1] && cp[9]) || (cp[3] && cp[11])) && (!cp[5] || !cp[7]) ){
                blockRight = true;
            }
            if( (cp[0] && cp[4] && cp[8] && !cp[1] && !cp[2])||(cp[0] && cp[1] && cp[2]) ){ // bei hoher Auslastung
                blockUp = true;
                blockLeft = true;
            }
            else if( (cp[1] && cp[5] && cp[9] && !cp[0] && !cp[3])||(cp[1] && cp[0] && cp[3]) ){
                blockUp = true;
                blockRight = true;
            }
            else if( (cp[2] && cp[6] && cp[10] && !cp[0] && !cp[3])||(cp[2] && cp[0] && cp[3]) ){
                blockDown = true;
                blockLeft = true;
            }
            else if( (cp[3] && cp[7] && cp[11] && !cp[1] && !cp[2])||(cp[3] && cp[1] && cp[2]) ){
                blockDown = true;
                blockRight = true;
            }
            else if( cp[0] && cp[1] && !cp[2] && !cp[3] ){
                blockUp = true;
            }
            else if( cp[2] && cp[3] && !cp[0] && !cp[1] ){
                blockDown = true;
            }
            else if( cp[0] && cp[2] && !cp[1] && !cp[3] ){
                blockLeft = true;
            }
            else if( cp[1] && cp[3] && !cp[0] && !cp[2] ){
                blockRight = true;
            }
        }
        
        // Monsterbewegung

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
				if(moveDirection == 0 && !blockUp){
                    walking = true;
					PosY -= (Speed*ElapsedTime);
                    sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*1,TILESIZE,TILESIZE*2));
                    blockDown = false;
				}
                else if(moveDirection == 1 && !blockDown){
                    walking = true;
					PosY += (Speed*ElapsedTime);
                    sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),0,TILESIZE,TILESIZE*2));
                    blockUp = false;
				}
                else if(moveDirection == 2 && !blockLeft){
                    walking = true;
					PosX -= (Speed*ElapsedTime);
                    sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*2,TILESIZE,TILESIZE*2));
                    blockRight = false;
				}
                else if(moveDirection == 3 && !blockRight){
                    walking = true;
					PosX += (Speed*ElapsedTime);
                    sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*3,TILESIZE,TILESIZE*2));
                    blockLeft = false;
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
            if( (y < -MOVETOLLERANCE) && !blockUp ){
                walking = true;
				PosY -= (Speed*ElapsedTime);
                sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*1,TILESIZE,TILESIZE*2));
                blockDown = false;
			}
			else if( (y > MOVETOLLERANCE) && !blockDown ){
                walking = true;
				PosY += (Speed*ElapsedTime);
                sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),0,TILESIZE,TILESIZE*2));
                blockUp = false;
			}
            if ( (x < -MOVETOLLERANCE) && !blockLeft ){
                walking = true;
				PosX -= (Speed*ElapsedTime);
                sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*2,TILESIZE,TILESIZE*2));
                blockRight = false;
			}
			else if( (x > MOVETOLLERANCE) && !blockRight ){
                walking = true;
				PosX += (Speed*ElapsedTime);
                sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*3,TILESIZE,TILESIZE*2));
                blockLeft = false;
			}

		}
        if( walking ){      // nur animieren wenn spieler lŠuft
            if( (Animation/(int)((1/Speed)*ElapsedTime*ANIMATIONSPEED)) >= 4) Animation = 0;
            Animation++;
        }

        // Kollision ggf. Korrigieren
        if( blockUp ){
            if( (TILESIZE-((int)PosY-(((int)PosY/TILESIZE)*TILESIZE))) > COLLISIONTOLERANCE ){
                PosY= (float)(((int)PosY/TILESIZE)*TILESIZE+TILESIZE-1);
            }
        }
        if( blockDown ){
            if( (((int)PosY+TILESIZE)-((((int)PosY+TILESIZE)/TILESIZE)*TILESIZE)) > COLLISIONTOLERANCE ){
                PosY= (float)((((int)PosY+TILESIZE)/TILESIZE)*TILESIZE-TILESIZE+1);
            }
        }
        if( blockLeft ){
            if( (TILESIZE-(((int)PosX-TILESIZE/2)-((((int)PosX-TILESIZE/2)/TILESIZE)*TILESIZE))) > COLLISIONTOLERANCE ){
                PosX= (float)((((int)PosX-TILESIZE/2)/TILESIZE)*TILESIZE+TILESIZE*3/2-1);
            }
        }
        if( blockRight ){
            if( (((int)PosX+TILESIZE/2)-((((int)PosX+TILESIZE/2)/TILESIZE)*TILESIZE)) > COLLISIONTOLERANCE ){
                PosX= (float)((((int)PosX+TILESIZE/2)/TILESIZE)*TILESIZE-TILESIZE/2+1);
            }
        }
        
		sprite.setPosition(PosX,PosY);
	}
}
