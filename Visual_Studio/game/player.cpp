#include <math.h>
#include "player.h"

Player::Player(sf::IntRect*** CollisionMap, int controller){
	
	this->ColMap = CollisionMap;

	this->pHealth = Savegame::currentSaveGame->pHealth;
	this->pLvl = Savegame::currentSaveGame->pLvl;
	this->pExp = Savegame::currentSaveGame->pExp;
	this->pGender = Savegame::currentSaveGame->pGender;
	this->pName = Savegame::currentSaveGame->pName;

	this->pHealthMax = BASEHEALTH-HEALTHPERLEVEL+this->pLvl*HEALTHPERLEVEL;
	this->pExpMax = BASEEXP*(int)pow(EXPMULTIPLICATOR,(this->pLvl-1));

	this->controller = controller;
	this->blockControl = false;

	sf::String tex;
	if(pGender == 'F'){         // spielertextur abhängig vom gewählten helden.
		#ifdef DEBUGINFO
			tex.insert(0,PATH"include/texture/player/player_female.png");
		#else
			tex.insert(0,PATH"include/texture/player/player_female.png");
		#endif
	}else {
		#ifdef DEBUGINFO
			tex.insert(0,PATH"include/texture/player/player_male.png");
		#else
			tex.insert(0,PATH"include/texture/player/player_male.png");
		#endif
	}

	Speed = PLAYERSPEED;
	HealTickRate = 0;
	Animation = 0;

	if(!texture.loadFromFile(tex)){
		throw "Error: Playertexture not found.";
	}
	#ifdef DEBUGINFO
	else {
		std::cout << "Successfully loaded the texture!" << std::endl;
	}
	#endif

	sprite.setTexture(texture);
	sprite.setOrigin(TILESIZE/2,TILESIZE);
	sprite.setTextureRect(sf::IntRect(0,0,TILESIZE,TILESIZE*2));
	sprite.setPosition(Savegame::currentSaveGame->mPosX, Savegame::currentSaveGame->mPosY);
}

void Player::Update(float ElapsedTime){
	
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
		{(int)x-TILESIZE/2, (int)y},									//  0: Links Oben						+--- Player ----+
		{(int)x+TILESIZE/2-1, (int)y},									//  1: Rechts Oben						|				|
		{(int)x-TILESIZE/2, (int)y+TILESIZE-1},							//  2: Links Unten						|				|
		{(int)x+TILESIZE/2-1, (int)y+TILESIZE-1},						//  3: Rechts Unten						|				| keine Kollisionsabfrage im oberen bereich.
		{(int)x-TILESIZE/2+COLLISIONTOLERANCE, (int)y},					//  4: Links Oben Hilfspunkt Oben		|				|
		{(int)x+TILESIZE/2-1-COLLISIONTOLERANCE, (int)y},				//  5: Rechts Oben Hilfspunkt Oben		|				|
		{(int)x-TILESIZE/2+COLLISIONTOLERANCE, (int)y+TILESIZE-1},		//  6: Links Unten Hilfspunkt Unten		| 00 04   05 01 |
		{(int)x+TILESIZE/2-1-COLLISIONTOLERANCE, (int)y+TILESIZE-1},	//  7: Rechts Unten Hilfspunkt Unten	| 08         09 |
		{(int)x-TILESIZE/2, (int)y+COLLISIONTOLERANCE},					//  8: Links Oben Hilfspunkt Links		|				|
		{(int)x+TILESIZE/2-1, (int)y+COLLISIONTOLERANCE},				//  9: Rechts Oben Hilfspunkt Rechts	| 10         11 |
		{(int)x-TILESIZE/2, (int)y+TILESIZE-1-COLLISIONTOLERANCE},		// 10: Links Unten Hilfspunkt Links		| 02 06   07 03 |
		{(int)x+TILESIZE/2-1, (int)y+TILESIZE-1-COLLISIONTOLERANCE},	// 11: Rechts Unten Hilfspunkt Rechts	+---------------+

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
		else if( cp[0] && cp[1] && cp[2] && cp[3] ){
			std::cout << "KOLLISION ÜBERALL" << std::endl;	// teleport an sicheren ort
		}
	}
            
	bool walking = false;
	if(!blockControl){
		if( sf::Joystick::isConnected(controller) ){    // analoge axen des controllers nur nutzen, falls auch einer angeschlossen ist
			if( (sf::Joystick::getAxisPosition(controller,sf::Joystick::Y) < -CONTROLLERTOLERANCE) && !blockUp ){
				y += (Speed*ElapsedTime*sf::Joystick::getAxisPosition(controller,sf::Joystick::Y)/100);
				blockDown = false;
				sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*3,TILESIZE,TILESIZE*2));
				walking = true;
			}
			if( (sf::Joystick::getAxisPosition(controller,sf::Joystick::Y) > CONTROLLERTOLERANCE) && !blockDown ){
				y += (Speed*ElapsedTime*sf::Joystick::getAxisPosition(controller,sf::Joystick::Y)/100);
				blockUp = false;
				sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),0,TILESIZE,TILESIZE*2));
				walking = true;
			}
			if( (sf::Joystick::getAxisPosition(controller,sf::Joystick::X) < -CONTROLLERTOLERANCE) && !blockLeft ){
				x += (Speed*ElapsedTime*sf::Joystick::getAxisPosition(controller,sf::Joystick::X)/100);
				blockRight = false;
				sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*1,TILESIZE,TILESIZE*2));
				walking = true;
			}
			if( (sf::Joystick::getAxisPosition(controller,sf::Joystick::X) > CONTROLLERTOLERANCE) && !blockRight ){
				x += (Speed*ElapsedTime*sf::Joystick::getAxisPosition(controller,sf::Joystick::X)/100);
				blockLeft = false;
				sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*2,TILESIZE,TILESIZE*2));
				walking = true;
			}
		}                       // hier nochmal die tastenabfragen. siehe menu.cpp ganz unten für tastenbelegung
		if( (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
			 sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
			 sf::Joystick::isButtonPressed(controller,11) )
		   && !(sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		   && !blockUp ){
			y -= (Speed*ElapsedTime);
			blockDown = false;
			sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*3,TILESIZE,TILESIZE*2));
			walking = true;
		}    
		if( (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
			 sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
			 sf::Joystick::isButtonPressed(controller,12))
		   && !(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		   && !blockDown ){
			y += (Speed*ElapsedTime);
			blockUp = false;
			sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),0,TILESIZE,TILESIZE*2));
			walking = true;
		}
		if ( (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
			  sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
			  sf::Joystick::isButtonPressed(controller,13))
			&& !(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			&& !blockLeft ){
			x -= (Speed*ElapsedTime);
			blockRight = false;
			sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*1,TILESIZE,TILESIZE*2));
			walking = true;
		}
		if( (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
			 sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
			 sf::Joystick::isButtonPressed(controller,14))
		   && !(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		   && !blockRight ){
			x += Speed*ElapsedTime;
			blockLeft = false;
			sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*2,TILESIZE,TILESIZE*2));
			walking = true;
		}
	}

	if( walking ){      // nur animieren wenn spieler läuft
		if( (Animation/(int)((1/Speed)*ElapsedTime*ANIMATIONSPEED)) >= 4) Animation = 0;
		Animation++;
		HealTickRate = -COOLDOWN + IDLEHEAL;	// Cool Down Phase, erst danach werden die HP regeneriert

	}
	else {
		sprite.setTextureRect(sf::IntRect(0,sprite.getTextureRect().top,TILESIZE,TILESIZE*2));  // spieler "steht", wenn er sich nicht bewegt.
        if( HealTickRate >= IDLEHEAL ){
			HealTickRate = 0;
			this->playerHeal((int)(  (this->pHealthMax/100) ));     // Heilt den Player wenn er sich nicht bewegt.
		}
		//std::cout << HealTickRate << std::endl;
		HealTickRate++;
	}
    
    // falls das spiel lagt, hier korrekturen für die kollisionsabfrage.
	if( blockUp ){
		if( (TILESIZE-((int)y-(((int)y/TILESIZE)*TILESIZE))) > COLLISIONTOLERANCE ){
			y= (float)(((int)y/TILESIZE)*TILESIZE+TILESIZE-1);
			#ifdef DEBUGINFO
				std::cout << "Kolliskorrektur: OBEN - " << y << std::endl;
			#endif
		}
	}
	if( blockDown ){
		if( (((int)y+TILESIZE)-((((int)y+TILESIZE)/TILESIZE)*TILESIZE)) > COLLISIONTOLERANCE ){
			y= (float)((((int)y+TILESIZE)/TILESIZE)*TILESIZE-TILESIZE+1);
			#ifdef DEBUGINFO
				std::cout << "Kolliskorrektur: UNTEN - " << y << std::endl;
			#endif
		}
	}
	if( blockLeft ){
		if( (TILESIZE-(((int)x-TILESIZE/2)-((((int)x-TILESIZE/2)/TILESIZE)*TILESIZE))) > COLLISIONTOLERANCE ){
			x= (float)((((int)x-TILESIZE/2)/TILESIZE)*TILESIZE+TILESIZE*3/2-1);
			#ifdef DEBUGINFO
				std::cout << "Kolliskorrektur: LINKS - " << x << std::endl;
			#endif
		}
	}
	if( blockRight ){
		if( (((int)x+TILESIZE/2)-((((int)x+TILESIZE/2)/TILESIZE)*TILESIZE)) > COLLISIONTOLERANCE ){
			x= (float)((((int)x+TILESIZE/2)/TILESIZE)*TILESIZE-TILESIZE/2+1);
			#ifdef DEBUGINFO
				std::cout << "Kolliskorrektur: RECHTS - " << x << std::endl;
			#endif
		}
	}

	sprite.setPosition(x,y);
}
