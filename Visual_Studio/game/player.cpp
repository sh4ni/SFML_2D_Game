﻿#include "player.h"


Player::Player(sf::IntRect*** CollisionMap, Savegame& currentSavegame){

	this->ColMap = CollisionMap;
	
	this->pHealth = currentSavegame.pHealth;
	this->pLvl = currentSavegame.pLvl;
	this->pExp = currentSavegame.pExp;
	this->pGender = currentSavegame.pGender;

	sf::String tex;
	if(pGender == 'F'){
		#ifdef DEBUG
			tex.insert(0,"include/texture/player/player_female.png");
		#else
			tex.insert(0,"include/texture/player/player_female.png");
		#endif
	}else {
		#ifdef DEBUG
			tex.insert(0,"include/texture/player/player_male.png");
		#else
			tex.insert(0,"include/texture/player/player_male.png");
		#endif
	}

	Speed = PLAYERSPEED;

	if(!texture.loadFromFile(tex)){
		std::cout << "Fehler beim Laden der Textur!" << std::endl;	
	}
	#ifdef DEBUG
	else {
		std::cout << "Textur wurde erfolgreich geladen!" << std::endl;
	}
	#endif

	//texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setOrigin(16.f,32.f);
	sprite.setTextureRect(sf::IntRect(0.f,0.f,TILESIZE,TILESIZE*2));
	sprite.setPosition(currentSavegame.mPosX,currentSavegame.mPosY);
	//sprite.setPosition(WIDTH/2,HEIGHT/2);
	//sprite.setScale(2.1f,2.1f); // player wird 110% groß skaliert

}

void Player::Update(sf::RenderWindow& Window, float ElapsedTime){
	
	this->x = sprite.getPosition().x;
	this->y = sprite.getPosition().y;
	
	int tx = ((int)x/TILESIZE)-1;
	int ty = ((int)y/TILESIZE)-1;

	bool blockUp = false;
	bool blockDown = false;
	bool blockLeft = false;
	bool blockRight = false;

	// 12 Kollisionspunkte
	int CollisionPoint[12][2] = {
		{x-TILESIZE/2, y},									//  0: Links Oben						+--- Player ----+
		{x+TILESIZE/2-1, y},								//  1: Rechts Oben						|				|
		{x-TILESIZE/2, y+TILESIZE-1},						//  2: Links Unten						|				|
		{x+TILESIZE/2-1, y+TILESIZE-1},						//  3: Rechts Unten						|				| keine Kollisionsabfrage im oberen bereich.
		{x-TILESIZE/2+COLLISIONTOLERANCE, y},				//  4: Links Oben Hilfspunkt Oben		|				|
		{x+TILESIZE/2-1-COLLISIONTOLERANCE, y},				//  5: Rechts Oben Hilfspunkt Oben		|				|
		{x-TILESIZE/2+COLLISIONTOLERANCE, y+TILESIZE-1},	//  6: Links Unten Hilfspunkt Unten		| 00 04   05 01 |
		{x+TILESIZE/2-1-COLLISIONTOLERANCE, y+TILESIZE-1},	//  7: Rechts Unten Hilfspunkt Unten	| 08         09 |
		{x-TILESIZE/2, y+COLLISIONTOLERANCE},				//  8: Links Oben Hilfspunkt Links		|				|
		{x+TILESIZE/2-1, y+COLLISIONTOLERANCE},				//  9: Rechts Oben Hilfspunkt Rechts	| 10         11 |
		{x-TILESIZE/2, y+TILESIZE-1-COLLISIONTOLERANCE},	// 10: Links Unten Hilfspunkt Links		| 02 06   07 03 |
		{x+TILESIZE/2-1, y+TILESIZE-1-COLLISIONTOLERANCE},	// 11: Rechts Unten Hilfspunkt Rechts	+---------------+

	};

	bool cp[12] = {0,0,0,0,0,0,0,0,0,0,0,0};

	for( int i=0; i<9; i++){
		if(tx+(i%3) > 0 && ty+i/3 > 0 && ColMap[tx+(i%3)][ty+i/3] != NULL){
			for( int p=0; p<12; p++){
				if( ColMap[tx+(i%3)][ty+i/3]->contains(CollisionPoint[p][0],CollisionPoint[p][1]) ){
					cp[p] = 1;
				}
			}
		}
	}

	for( int i=0; i<12; i++){	// Punktabfrage ist noch immer nicht wirklich toll... Benötigt auf jedenfall noch etwas Arbeit!
		//std::cout << cp[i];
		if( (cp[0] && cp[4] || cp[1] && cp[5]) && (!cp[8] || !cp[9]) ){ // Bei Normaler Auslastung
			blockUp = true;
		}
		else if( (cp[2] && cp[6] || cp[3] && cp[7]) && (!cp[10] || !cp[11]) ){
			blockDown = true;
		}
		if( (cp[0] && cp[8] || cp[2] && cp[10]) && (!cp[4] || !cp[6]) ){
			blockLeft = true;
		}
		else if( (cp[1] && cp[9] || cp[3] && cp[11]) && (!cp[5] || !cp[7]) ){
			blockRight = true;
		}
		if( cp[0] && cp[4] && cp[8] && !cp[1] && !cp[2] ){ // bei hoher Auslastung
			blockUp = true;
			blockLeft = true;
		}
		else if( cp[1] && cp[5] && cp[9] && !cp[0] && !cp[3] ){
			blockUp = true;
			blockRight = true;
		}
		else if( cp[2] && cp[6] && cp[10] && !cp[0] && !cp[3] ){
			blockDown = true;
			blockLeft = true;
		}
		else if( cp[3] && cp[7] && cp[11] && !cp[1] && !cp[2] ){
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

	if( (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) && !blockUp ){
		y -= (Speed*ElapsedTime);
		blockDown = false;
		sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)(Speed*ANIMATIONSPEED))%3),TILESIZE*2*3,TILESIZE,TILESIZE*2));
		Animation++;
	}
	if( (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) && !blockDown ){
		y += (Speed*ElapsedTime);
		blockUp = false;
		sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)(Speed*ANIMATIONSPEED))%3),0,TILESIZE,TILESIZE*2));
		Animation++;
	}
	if ( (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) && !blockLeft ){
		x -= (Speed*ElapsedTime);
		blockRight = false;
		sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)(Speed*ANIMATIONSPEED))%3),TILESIZE*2*1,TILESIZE,TILESIZE*2));
		Animation++;
	}
	if( (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && !blockRight ){
		x += Speed*ElapsedTime;
		blockLeft = false;
		sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)(Speed*ANIMATIONSPEED))%3),TILESIZE*2*2,TILESIZE,TILESIZE*2));
		Animation++;
	}

	if( blockUp ){
		if( (TILESIZE-((int)y-(((int)y/TILESIZE)*TILESIZE))) > COLLISIONTOLERANCE ){
			y= (float)(((int)y/TILESIZE)*TILESIZE+TILESIZE-1);
			#ifdef DEBUG
				std::cout << "Kolliskorrektur: OBEN - " << y << std::endl;
			#endif
		}
	}
	if( blockDown ){
		if( (((int)y+TILESIZE)-((((int)y+TILESIZE)/TILESIZE)*TILESIZE)) > COLLISIONTOLERANCE ){
			y= (float)((((int)y+TILESIZE)/TILESIZE)*TILESIZE-TILESIZE+1);
			#ifdef DEBUG
				std::cout << "Kolliskorrektur: UNTEN - " << y << std::endl;
			#endif
		}
	}
	if( blockLeft ){
		if( (TILESIZE-(((int)x-TILESIZE/2)-((((int)x-TILESIZE/2)/TILESIZE)*TILESIZE))) > COLLISIONTOLERANCE ){
			x= (float)((((int)x-TILESIZE/2)/TILESIZE)*TILESIZE+TILESIZE*3/2-1);
			#ifdef DEBUG
				std::cout << "Kolliskorrektur: LINKS - " << x << std::endl;
			#endif
		}
	}
	if( blockRight ){
		if( (((int)x+TILESIZE/2)-((((int)x+TILESIZE/2)/TILESIZE)*TILESIZE)) > COLLISIONTOLERANCE ){
			x= (float)((((int)x+TILESIZE/2)/TILESIZE)*TILESIZE-TILESIZE/2+1);
			#ifdef DEBUG
				std::cout << "Kolliskorrektur: RIGHT - " << x << std::endl;
			#endif
		}
	}

	sprite.setPosition(x,y);

}
