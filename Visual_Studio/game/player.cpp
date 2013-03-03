#include <math.h>
#include "savegame.h"
#include "player.h"

Player::Player(int controller){
    Init(controller);
}

void Player::Init(int controller){
	
	this->Health = Savegame::currentSaveGame->pHealth;
	this->Lvl = Savegame::currentSaveGame->pLvl;
	this->pExp = Savegame::currentSaveGame->pExp;
	this->pGender = Savegame::currentSaveGame->pGender;
	this->Name = Savegame::currentSaveGame->pName;
	this->LevelId = Savegame::currentSaveGame->mLevelId;
	this->pHealthMax = (int)(BASEHEALTH*pow(HEALTHMULTIPLICATOR,(float)(this->Lvl-1)));
	this->pExpMax = (int)(BASEEXP*pow(EXPMULTIPLICATOR,(float)(this->Lvl-1)));
    this->AttackPower = (int)(BASEDMG*pow(DMGMULTIPLICATOR,(float)(this->Lvl-1)));

	this->controller = controller;
	this->blockControl = false;
	this->isAttacking = false;
	this->canAttack = true;
	this->lookDirection = 'D';

	sf::String tex;
	if(pGender == 'F'){         // spielertextur abhängig vom gewählten helden.
		tex.insert(0,PATH"include/texture/player/player_female.png");
	}else {
		tex.insert(0,PATH"include/texture/player/player_male.png");
	}

	if(!texture.loadFromFile(tex)){
		throw "Error: Playertexture not found.";
	}
	#ifdef DEBUGINFO
	else {
		std::cout << "Successfully loaded the player texture!" << std::endl;
	}
	#endif
	
	tex.clear();
	tex.insert(0,PATH"include/texture/player/weapon_sword.png");
	if(!weaponTexture.loadFromFile(tex)){
		throw "Error: Weapontexture not found.";
	}
	#ifdef DEBUGINFO
	else {
		std::cout << "Successfully loaded the weapon texture!" << std::endl;
	}
	#endif

	Speed = PLAYERSPEED;
	HealTickRate = 0;
	Animation = 0;

	sprite.setTexture(texture);
	sprite.setOrigin(TILESIZE/2,TILESIZE);
	sprite.setTextureRect(sf::IntRect(0,0,TILESIZE,TILESIZE*2));
	sprite.setPosition(Savegame::currentSaveGame->mPosX, Savegame::currentSaveGame->mPosY);

	weaponSprite.setTexture(weaponTexture);
	weaponSprite.setOrigin(8,0);
	weaponSprite.setTextureRect(sf::IntRect(0,0,16,32));
	weaponSprite.setPosition(Savegame::currentSaveGame->mPosX, Savegame::currentSaveGame->mPosY+TILESIZE/2);
}

void Player::Update(float ElapsedTime){

	this->PosX = sprite.getPosition().x;
	this->PosY = sprite.getPosition().y;

	int tx = ((int)PosX/TILESIZE)-1;
	int ty = ((int)PosY/TILESIZE)-1;

	bool blockUp = false;
	bool blockDown = false;
	bool blockLeft = false;
	bool blockRight = false;

	// 12 Kollisionspunkte
	int CollisionPoint[12][2] = {
		{(int)PosX-TILESIZE/2, (int)PosY},										//  0: Links Oben						+--- Player ----+
		{(int)PosX+TILESIZE/2-1, (int)PosY},									//  1: Rechts Oben						|				|
		{(int)PosX-TILESIZE/2, (int)PosY+TILESIZE-1},							//  2: Links Unten						|				|
		{(int)PosX+TILESIZE/2-1, (int)PosY+TILESIZE-1},							//  3: Rechts Unten						|				| keine Kollisionsabfrage im oberen bereich.
		{(int)PosX-TILESIZE/2+COLLISIONTOLERANCE, (int)PosY},					//  4: Links Oben Hilfspunkt Oben		|				|
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
		else if( cp[0] && cp[1] && cp[2] && cp[3] ){
			std::cout << "KOLLISION ÜBERALL" << std::endl;	// teleport an sicheren ort
		}
	}
            
	bool walking = false;
	if(!blockControl){
		if( sf::Joystick::isConnected(controller) ){    // analoge axen des controllers nur nutzen, falls auch einer angeschlossen ist
			if( (sf::Joystick::getAxisPosition(controller,sf::Joystick::Y) < -CONTROLLERTOLERANCE) && !blockUp ){
				PosY += (Speed*ElapsedTime*sf::Joystick::getAxisPosition(controller,sf::Joystick::Y)/100);
				if(!isAttacking) lookDirection = 'U';
				blockDown = false;
				walking = true;
			}
			if( (sf::Joystick::getAxisPosition(controller,sf::Joystick::Y) > CONTROLLERTOLERANCE) && !blockDown ){
				PosY += (Speed*ElapsedTime*sf::Joystick::getAxisPosition(controller,sf::Joystick::Y)/100);
				if(!isAttacking) lookDirection = 'D';
				blockUp = false;
				walking = true;
			}
			if( (sf::Joystick::getAxisPosition(controller,sf::Joystick::X) < -CONTROLLERTOLERANCE) && !blockLeft ){
				PosX += (Speed*ElapsedTime*sf::Joystick::getAxisPosition(controller,sf::Joystick::X)/100);
				if(!isAttacking) lookDirection = 'L';
				blockRight = false;
				walking = true;
			}
			if( (sf::Joystick::getAxisPosition(controller,sf::Joystick::X) > CONTROLLERTOLERANCE) && !blockRight ){
				PosX += (Speed*ElapsedTime*sf::Joystick::getAxisPosition(controller,sf::Joystick::X)/100);
				if(!isAttacking) lookDirection = 'R';
				blockLeft = false;
				walking = true;
			}
		}                       // hier nochmal die tastenabfragen. siehe menu.cpp ganz unten für tastenbelegung
		if( (sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
			 sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
			 sf::Joystick::isButtonPressed(controller,11) )
		   && !(sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		   && !blockUp ){
			PosY -= (Speed*ElapsedTime);
			if(!isAttacking) lookDirection = 'U';
			blockDown = false;
			walking = true;
		}    
		if( (sf::Keyboard::isKeyPressed(sf::Keyboard::S) ||
			 sf::Keyboard::isKeyPressed(sf::Keyboard::Down) ||
			 sf::Joystick::isButtonPressed(controller,12))
		   && !(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		   && !blockDown ){
			PosY += (Speed*ElapsedTime);
			if(!isAttacking) lookDirection = 'D';
			blockUp = false;
			walking = true;
		}
		if ( (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
			  sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
			  sf::Joystick::isButtonPressed(controller,13))
			&& !(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			&& !blockLeft ){
			PosX -= (Speed*ElapsedTime);
			if(!isAttacking) lookDirection = 'L';
			blockRight = false;
			walking = true;
		}
		if( (sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
			 sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
			 sf::Joystick::isButtonPressed(controller,14))
		   && !(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
   		   && !blockRight ){
            PosX += Speed*ElapsedTime;
			if(!isAttacking) lookDirection = 'R';
			blockLeft = false;
			walking = true;
		}
	}

	if( (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) && canAttack ){
		canAttack = false;
		isAttacking = true;
	}
	if( !(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) && !canAttack ){
		canAttack = true;
		isAttacking = false;
	}

	if(lookDirection == 'U'){
		sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*1,TILESIZE,TILESIZE*2));
		weaponSprite.setRotation(180);
	}
	else if(lookDirection == 'D'){
		sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),0,TILESIZE,TILESIZE*2));
		weaponSprite.setRotation(0);
	}
	else if(lookDirection == 'L'){
		sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*2,TILESIZE,TILESIZE*2));
		weaponSprite.setRotation(90);
	}
	else {
		sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*3,TILESIZE,TILESIZE*2));
		weaponSprite.setRotation(270);
	}

	if( walking ){      // nur animieren wenn spieler läuft
		if( (Animation/(int)((1/Speed)*ElapsedTime*ANIMATIONSPEED)) >= 4) Animation = 0;
		Animation++;
		ResetCooldown();	// Cool Down Phase, erst danach werden die HP regeneriert
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
		if( (TILESIZE-((int)PosY-(((int)PosY/TILESIZE)*TILESIZE))) > COLLISIONTOLERANCE ){
			PosY= (float)(((int)PosY/TILESIZE)*TILESIZE+TILESIZE-1);
			#ifdef DEBUGINFO
				std::cout << "Kolliskorrektur: OBEN - " << PosY << std::endl;
			#endif
		}
	}
	if( blockDown ){
		if( (((int)PosY+TILESIZE)-((((int)PosY+TILESIZE)/TILESIZE)*TILESIZE)) > COLLISIONTOLERANCE ){
			PosY= (float)((((int)PosY+TILESIZE)/TILESIZE)*TILESIZE-TILESIZE+1);
			#ifdef DEBUGINFO
				std::cout << "Kolliskorrektur: UNTEN - " << PosY << std::endl;
			#endif
		}
	}
	if( blockLeft ){
		if( (TILESIZE-(((int)PosX-TILESIZE/2)-((((int)PosX-TILESIZE/2)/TILESIZE)*TILESIZE))) > COLLISIONTOLERANCE ){
			PosX= (float)((((int)PosX-TILESIZE/2)/TILESIZE)*TILESIZE+TILESIZE*3/2-1);
			#ifdef DEBUGINFO
				std::cout << "Kolliskorrektur: LINKS - " << PosX << std::endl;
			#endif
		}
	}
	if( blockRight ){
		if( (((int)PosX+TILESIZE/2)-((((int)PosX+TILESIZE/2)/TILESIZE)*TILESIZE)) > COLLISIONTOLERANCE ){
			PosX= (float)((((int)PosX+TILESIZE/2)/TILESIZE)*TILESIZE-TILESIZE/2+1);
			#ifdef DEBUGINFO
				std::cout << "Kolliskorrektur: RECHTS - " << PosX << std::endl;
			#endif
		}
	}
	weaponSprite.setPosition(PosX,PosY+TILESIZE/2);

	if(isAttacking){
		weaponDmgBox = weaponSprite.getGlobalBounds();
        //weaponDmgBox.top += TILESIZE/2;
		for( int i=0; i < Map::currentMap->getMonsterCounter(); i++){
			Monster* mon = Map::currentMap->getMonsterList();
			if( mon[i].getHitBox().intersects(weaponDmgBox) ){
				isAttacking = false;
				std::cout << "HIT: " << i << std::endl;
                mon[i].damageMe(AttackPower,this->Lvl);
                mon[i].targetPlayer();

			}
		}
		//std::cout << "x: " << weaponDmgBox.left << " y: " << weaponDmgBox.top << " b: " << weaponDmgBox.width << " h: " << weaponDmgBox.height << std::endl;
	}


	sprite.setPosition(PosX,PosY);
}
