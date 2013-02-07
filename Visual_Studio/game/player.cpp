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

	CollisionX.height = TILESIZE*2-COLLISIONTOLERANCE*2;
	CollisionX.width = TILESIZE;

	CollisionY.height = TILESIZE*2;
	CollisionY.width = TILESIZE-COLLISIONTOLERANCE*2;

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

	CollisionX.left = (int)x-TILESIZE/2;
	CollisionX.top = (int)y-TILESIZE+COLLISIONTOLERANCE;

	CollisionY.left = (int)x-TILESIZE/2+COLLISIONTOLERANCE;
	CollisionY.top = (int)y-TILESIZE;

	bool blockUp = false;
	bool blockDown = false;
	bool blockLeft = false;
	bool blockRight = false;

	for(int i=0;i<9;i++){
		if(tx+(i%3) > 0 && ty+i/3 > 0 && ColMap[tx+(i%3)][ty+i/3] != NULL){
			if(CollisionX.intersects(*ColMap[tx+(i%3)][ty+i/3])){
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
	}

	if( (Keyboard::isKeyPressed(Keyboard::W) || Keyboard::isKeyPressed(Keyboard::Up)) && !blockUp ){
		y -= (Speed*ElapsedTime);
	}
	if( (Keyboard::isKeyPressed(Keyboard::S) || Keyboard::isKeyPressed(Keyboard::Down)) && !blockDown ){
		y += (Speed*ElapsedTime);
	}
	if ( (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left)) && !blockLeft ){
		x -= (Speed*ElapsedTime);
	}
	if( (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right)) && !blockRight ){
		x += Speed*ElapsedTime;
	}

	if( blockUp ){
		if( ((TILESIZE-(((int)y-TILESIZE)-((((int)y-TILESIZE)/TILESIZE)*TILESIZE))) > COLLISIONTOLERANCE) && ((TILESIZE-(((int)y-TILESIZE)-((((int)y-TILESIZE)/TILESIZE)*TILESIZE))) < TILESIZE) ){
			std::cout << (((int)y-TILESIZE)/TILESIZE)*TILESIZE+TILESIZE*2-1 << " ";
		}
	}

	sprite.setPosition(x,y);

}

void Player::Render(RenderWindow &Window){
	Window.draw(sprite);
}