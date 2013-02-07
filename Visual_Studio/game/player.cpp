#include "defines.h"
#include "player.h"

Player::Player(String tex, IntRect*** CollisionMap){

	this->ColMap = CollisionMap;

	Speed = PLAYERSPEED;

	if(!texture.loadFromFile(tex)){
		std::cout << "Fehler beim Laden der Textur!" << std::endl;	
	}else{
		#ifdef DEBUG
			std::cout << "Textur wurde erfolgreich geladen!" << std::endl;
		#endif
	}

	//texture.setSmooth(true);
	sprite.setTexture(texture);
	sprite.setOrigin(16.f,32.f);
	sprite.setPosition(WIDTH/2,HEIGHT/2);
	//sprite.setScale(2.1f,2.1f); // player wird 110% groß skaliert

	/*CollisionX.height = TILESIZE*2-COLLISIONTOLERANCE*2;
	CollisionX.width = TILESIZE;

	CollisionY.height = TILESIZE*2;
	CollisionY.width = TILESIZE-COLLISIONTOLERANCE*2;*/

	/*RectangleShape box(Vector2f(5.f,5.f));
	box.setPosition(50,50);
	box.setFillColor(Color(0,0,0));*/
}

float Player::getPosX(void){
	return this->x;
}

float Player::getPosY(void){
	return this->y;
}

void Player::Update(RenderWindow &Window, float ElapsedTime){
	
	this->x = sprite.getPosition().x;
	this->y = sprite.getPosition().y;
	
	int tx = ((int)x/TILESIZE)-1;
	int ty = ((int)y/TILESIZE)-1;

	/*CollisionX.left = (int)x-TILESIZE/2;
	CollisionX.top = (int)y-TILESIZE+COLLISIONTOLERANCE;

	CollisionY.left = (int)x-TILESIZE/2+COLLISIONTOLERANCE;
	CollisionY.top = (int)y-TILESIZE;*/

	bool blockUp = false;
	bool blockDown = false;
	bool blockLeft = false;
	bool blockRight = false;

	// 14 Kollisionspunkte
	int CollisionPoint[14][3] = {
		{x-TILESIZE/2, y-TILESIZE,0},						//  0: Links Oben
		{x+TILESIZE/2-1, y-TILESIZE,0},						//  1: Rechts Oben							Player
		{x-TILESIZE/2, y+TILESIZE-1,0},						//  2: Links Unten						+---------------+
		{x+TILESIZE/2-1, y+TILESIZE-1,0},					//  3: Rechts Unten						| 00 04   05 01 |
		{x-TILESIZE/2+COLLISIONTOLERANCE, y-TILESIZE,0},	//  4: Links Oben Hilfspunkt Oben		| 08         09 |
		{x+TILESIZE/2-1-COLLISIONTOLERANCE, y-TILESIZE,0},	//  5: Rechts Oben Hilfspunkt Oben		|				|
		{x-TILESIZE/2+COLLISIONTOLERANCE, y+TILESIZE-1,0},	//  6: Links Unten Hilfspunkt Unten		|				|
		{x+TILESIZE/2-1-COLLISIONTOLERANCE, y+TILESIZE-1,0},//  7: Rechts Unten Hilfspunkt Unten	| 12         13 |
		{x-TILESIZE/2, y-TILESIZE+COLLISIONTOLERANCE,0},	//  8: Links Oben Hilfspunkt Links		|				|
		{x+TILESIZE/2-1, y-TILESIZE+COLLISIONTOLERANCE,0},	//  9: Rechts Oben Hilfspunkt Rechts	|				|
		{x-TILESIZE/2, y+TILESIZE-1-COLLISIONTOLERANCE,0},	// 10: Links Unten Hilfspunkt Links		| 10         11 |
		{x+TILESIZE/2-1, y+TILESIZE-1-COLLISIONTOLERANCE,0},// 11: Rechts Unten Hilfspunkt Rechts	| 02 06   07 03 |
		{x-TILESIZE/2, y,0},								// 12: Links Mitte						+---------------+
		{x+TILESIZE/2-1, y,0},								// 13: Rechts Mitte

	};

	for( int i=0; i<9; i++){
		if(tx+(i%3) > 0 && ty+i/3 > 0 && ColMap[tx+(i%3)][ty+i/3] != NULL){
			for( int p=0; p<14; p++){
				if( ColMap[tx+(i%3)][ty+i/3]->contains(CollisionPoint[p][0],CollisionPoint[p][1]) ){
					CollisionPoint[p][2] = 1;
				}
			}
		}
	}



	/*for(int i=0;i<9;i++){
		if(tx+(i%3) > 0 && ty+i/3 > 0 && ColMap[tx+(i%3)][ty+i/3] != NULL){
			if(CollisionX.intersects(*ColMap[tx+(i%3)][ty+i/3])){
			std::cout << i;
				if (!(i%3)){ // left
					//x += (Speed*ElapsedTime);
					blockLeft = true;
				}
				if( !((i-2)%3) ){ // right
					//x -= (Speed*ElapsedTime);
					blockRight = true;
				}
			}
			if(CollisionY.intersects(*ColMap[tx+(i%3)][ty+i/3])){
			std::cout << i;
				if( i <= 2 ){ // top
					//y += (Speed*ElapsedTime);
					blockUp = true;
				}
				if( i >= 6){ // bottom
					//y -= (Speed*ElapsedTime);
					blockDown = true;
				}
			}
		}
	}*/

	if( (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) && !blockUp ){
		y -= (Speed*ElapsedTime);
		blockDown = false;
	}
	if( (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) && !blockDown ){
		y += (Speed*ElapsedTime);
		blockUp = false;
	}
	if ( (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) && !blockLeft ){
		x -= (Speed*ElapsedTime);
		blockRight = false;
	}
	if( (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) && !blockRight ){
		x += Speed*ElapsedTime;
		blockLeft = false;
	}

	if( blockUp ){
		if( (TILESIZE-(((int)y-TILESIZE)-((((int)y-TILESIZE)/TILESIZE)*TILESIZE))) > COLLISIONTOLERANCE ){
			y= (((int)y-TILESIZE)/TILESIZE)*TILESIZE+TILESIZE*2-1;
			#ifdef DEBUG
				std::cout << "Kolliskorrektur: OBEN - " << y << std::endl;
			#endif
		}
	}
	if( blockDown ){
		if( (((int)y+TILESIZE)-((((int)y+TILESIZE)/TILESIZE)*TILESIZE)) > COLLISIONTOLERANCE ){
			y= (((int)y+TILESIZE)/TILESIZE)*TILESIZE-TILESIZE+1;
			#ifdef DEBUG
				std::cout << "Kolliskorrektur: UNTEN - " << y << std::endl;
			#endif
		}
	}
	if( blockLeft ){
		if( (TILESIZE-(((int)x-TILESIZE/2)-((((int)x-TILESIZE/2)/TILESIZE)*TILESIZE))) > COLLISIONTOLERANCE ){
			x= (((int)x-TILESIZE/2)/TILESIZE)*TILESIZE+TILESIZE*3/2-1;
			#ifdef DEBUG
				std::cout << "Kolliskorrektur: LINKS - " << x << std::endl;
			#endif
		}
	}
	if( blockRight ){
		if( (((int)x+TILESIZE/2)-((((int)x+TILESIZE/2)/TILESIZE)*TILESIZE)) > COLLISIONTOLERANCE ){
			x= (((int)x+TILESIZE/2)/TILESIZE)*TILESIZE-TILESIZE/2+1;
			#ifdef DEBUG
				std::cout << "Kolliskorrektur: RIGHT - " << x << std::endl;
			#endif
		}
	}

	sprite.setPosition(x,y);

}

void Player::Render(RenderWindow &Window){
	Window.draw(sprite);
}