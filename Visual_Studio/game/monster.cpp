/**
	Verantwortlich: Daniel Maier <br>
	Infotext: Dies ist die Klasse für die Monster im Spiel.
    Die Monster werden einmalig Initialisiert und dann während des Gameloops immer
    aktualisiert.
    Die Monster bewegen sich eigenständig auf der Karte, können dem Spieler schaden
    zufügen und haben die selbe Kollisionsabfrage wie der Spieler auch.
    Genauere Infos über die Kollisionsabfrage gibt es in der player.cpp, diese wurde
    1zu1 ins Monster übertragen.
    Es gibt verschiedene Monstertypen und ein Monster kann besonders stark, ein Boss
    oder doppelt so groß sein.
    Es bewegt sich in einem zufälligen Zeitintervall in eine zufällige Richtung.
    Es kann Aggresiv oder Passiv sein.
    Aggresive Monster greifen den Spieler bei sichtkontakt an, andere nur wenn sie
    angegriffen werden.
    Flüchtet der Spieler von einem Monster, stellt dieses ab einem gewissen Abstand
    zum Spieler die verfolgung ein.
*/
#include "monster.h"
#include "map.h"

/// Der Konstruktor des Monsters setzt Default-Werte für Variablen.
Monster::Monster(){
	this->Speed = 0.1f;
	this->isActive = true;
    this->Animation = 0;
    this->isHit = false;
    this->hitTimer = 0.f;
    this->dmgTimer = 0.f;
    this->canAttack = true;
    this->isBig = false;
    this->isSpecial = 'n';
    
    this->hpBox.setFillColor(sf::Color(0x99,0x33,0x33));
    this->nameBox.Init(PosX,PosY,"Missing Name",12);
    this->levelBox.Init(PosX,PosY,"-1",12);
};

Monster::~Monster(){

};

/// Der Typ (bzw. die Art) des Monsters wird gesetzt.
void Monster::setType(int monsterType){
	this->monsterType = monsterType;
	Init();
}

/**
 Monster werden Initialisiert.
 Hier wird das Level, die Texture, Statuswerte und das Verhalten des Monsters bestimmt.
 */
void Monster::Init(){
    int minLevel = Map::currentMap->getMonsterLevel().x;
    int maxLevel = Map::currentMap->getMonsterLevel().y;
    
    this->Lvl = (rand()%(maxLevel-minLevel+1))+minLevel;
    this->AttackPower = FMONDMG;//(int)(MONSTERBASEDMG*pow(MONSTERDMGMULTIPLICATOR,(float)(this->Lvl-1)));
	this->Health = FMONHP;//(int)(MONSTERBASEHEALTH*pow(HEALTHMULTIPLICATOR,(float)(this->Lvl-1)));
    this->exp = FMONEXP;//(int)(MONSTERBASEEXP*pow(MONSTEREXPMULTIPLICATOR,(float)(this->Lvl-1)));

	sf::String tex;
	//std::cout << monsterType << std::endl;
    this->isAggressiv = true;
    switch(monsterType){
        case 1:
            this->Name = "Green Slime";
            tex.insert(0,PATH"include/texture/monster/entity_slime_green.png");
            this->isAggressiv = false;
            break;
        case 2:
            this->Name = "Red Slime";
            tex.insert(0,PATH"include/texture/monster/entity_slime_red.png");
            break;
        case 3:
            this->Name = "Blue Slime";
            tex.insert(0,PATH"include/texture/monster/entity_slime_blue.png");
            this->isSpecial = 'b';
            this->isBig = true;
            break;
        default:
            break;
    }
    nameBox.Update(Name);

    std::ostringstream monsterLevelText;
    //monsterLevelText.precision(0);
    monsterLevelText << std::fixed << Lvl;
    levelBox.Update(monsterLevelText.str());
    
    switch(isSpecial){  // b = Boss // e = Elite
        case 'b':
            this->AttackPower = (int)((float)this->AttackPower * 1.2f);
            this->Health *= 20;
            this->exp *= 25;
            this->Speed = 0.15f;
            break;
        case 'e':
            this->AttackPower = (int)((float)this->AttackPower * 1.5f);
            this->Health *= 2;
            this->exp *= 2;
            this->Speed = 0.2f;
            break;
    }
    
    maxHealth = Health;
    
	if(!texture.loadFromFile(tex)){
		throw "Error: Monstertexture not found.";
	}

	#ifdef DEBUGINFO
	else {
		//std::cout << "Successfully loaded the Monster texture!" << std::endl;
	}
	#endif

	sprite.setTexture(texture);
	sprite.setTextureRect(sf::IntRect(0,0,TILESIZE,TILESIZE*2));
    if(isBig){
        sprite.setScale(2,2);
        sprite.setOrigin(TILESIZE/2,TILESIZE+TILESIZE/2);
    }
    else {
        sprite.setOrigin(TILESIZE/2,TILESIZE);
        
    }
	sprite.setPosition(PosX, PosY);
	this->begin = clock();
    this->moveDirection = (int)rand() % 4;	// 0 up - 1 down - 2 left - 3 right

    this->hitBox.width = TILESIZE-TILESIZE/4;
    this->hitBox.height = TILESIZE-TILESIZE/4;

    if(isBig){
        this->hitBox.width = TILESIZE*2-8;
        this->hitBox.height = TILESIZE*2-8;
    }
    else {
        this->hitBox.width = TILESIZE-8;
        this->hitBox.height = TILESIZE-8;
    }
}

/**
 Die Update-Methode wird im Gameloop immer wieder neu aufgerufen.
 Sie bewegt die Monster korrekt, hat die selbe Kollisionsabfrage wie der Spieler und lässt Monster den Spieler angreifen.
 Sie berechnet das Verhalten des Monsters.
 */
void Monster::Update(float ElapsedTime){
	if(isActive){
		this->PosX = sprite.getPosition().x;
		this->PosY = sprite.getPosition().y;
        
		//Hitbox
        if(isBig){
            this->hitBox.left = PosX+4-TILESIZE;
            this->hitBox.top = PosY+8-TILESIZE;
        }
        else {
            this->hitBox.left = PosX+4-TILESIZE/2;
            this->hitBox.top = PosY+8;            
        }
        
#ifdef DEBUGINFO
        drawHitBox.setFillColor(sf::Color(255,255,255,75));
        drawHitBox.setPosition(hitBox.left, hitBox.top);
        drawHitBox.setSize(sf::Vector2f(hitBox.width,hitBox.height));
#endif
        
        hpBox.setPosition(PosX-50.f, PosY-10.f);
        hpBox.setSize(sf::Vector2f((float)Health/(float)maxHealth*100.f,14.f));
        
        nameBox.Position(PosX-48.f, PosY-10.f);
        levelBox.Position(PosX+40, PosY-10.f);
        
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
		if( elapsed_secs > ( (float)HOLDTIME + MOVETIME + ((float)(rand()%(HOLDTIME*500))/1000) ) ){
			this->begin = end;
			this->moveDirection = (int)rand() % 4;	// 0 up - 1 down - 2 left - 3 right
		}
        
		if(isAggressiv || targetingPlayer){
			float x = PlayerPosX - PosX;
			float y = PlayerPosY - PosY;

			if(x < 0) x = -x;
			if(y < 0) y = -y;

			x = sqrt(pow(x,2.f)+pow(y,2.f));

            if ( (x < (float)(DMGRANGE*TILESIZE+(isBig?(TILESIZE/2):0))) && canAttack ){
                Map::currentMap->getPlayer()->playerDamage(this->AttackPower, this->Lvl);
                canAttack = false;
            }
			if ( x < (float)(DETECTIONRADIUS*TILESIZE) ){
				this->targetingPlayer = true;
			}
            else if( x > LOSTRADIUS){
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
            
            if(isHit){
                if( isSpecial == 'b' || isSpecial == 'e' ){
                    Speed = 0.01f;
                }
                else {
                    Speed = 0.3f;
                    x = -x;
                    y = -y;                    
                }
            }
            else Speed = 0.1f;
            
            if( (y < -(MOVETOLLERANCE+(isBig?(TILESIZE/2):0))) && !blockUp ){
                walking = true;
				PosY -= (Speed*ElapsedTime);
                sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*1,TILESIZE,TILESIZE*2));
                blockDown = false;
			}
			else if( (y > MOVETOLLERANCE+(isBig?(TILESIZE/2):0)) && !blockDown ){
                walking = true;
				PosY += (Speed*ElapsedTime);
                sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),0,TILESIZE,TILESIZE*2));
                blockUp = false;
			}
            if ( (x < -(MOVETOLLERANCE+(isBig?(TILESIZE/2):0))) && !blockLeft ){
                walking = true;
				PosX -= (Speed*ElapsedTime);
                sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*2,TILESIZE,TILESIZE*2));
                blockRight = false;
			}
			else if( (x > MOVETOLLERANCE+(isBig?(TILESIZE/2):0)) && !blockRight ){
                walking = true;
				PosX += (Speed*ElapsedTime);
                sprite.setTextureRect(sf::IntRect(TILESIZE*((Animation/(int)((1/Speed)*ANIMATIONSPEED))%4+1),TILESIZE*2*3,TILESIZE,TILESIZE*2));
                blockLeft = false;
			}
		}
        
        if( walking ){      // nur animieren wenn spieler läuft
            if( (Animation/(int)((1/Speed)*ElapsedTime*ANIMATIONSPEED)) >= 4) Animation = 0;
            Animation++;
        }
        
        if(isHit){
            hitTimer += ElapsedTime/10.f;
            if( hitTimer > ( (isSpecial=='e'||isSpecial=='b')?96.f:8.f) ){
                hitTimer = 0.f;
                isHit = false;
            }
        }
        
        if(!canAttack){
            dmgTimer += ElapsedTime/10.f;
            if( dmgTimer > 64.f){
                dmgTimer = 0.f;
                canAttack = true;
            }
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

/// Diese Methode fügt dem Monster Schaden zu, wenn der Spieler es Angreift.
void Monster::damageMe( int damage, int level ){
    isHit = true;
    damage = calcDamage(damage, level);
    damageText(damage,'m');
    this->Health -= damage;
    if( this->Health <= 0 ){
        isActive = false;
        Map::currentMap->getPlayer()->playerExp(this->exp,this->Lvl);
        hitBox.left = 0;
        hitBox.top = 0;
        hitBox.width = 0;
        hitBox.height = 0;
    }
}